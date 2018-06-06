module type ClauseType = {
  let prefix: string;

  let separator: string;
};

let addToMaybeList = (maybeList, thing) =>
  switch (maybeList) {
  | Some(list) => Some(Belt.List.add(list, thing))
  | None => Some(Belt.List.make(1, thing))
  };

module Clause = (Type: ClauseType) => {
  type t = Belt.List.t(string);

  let add = addToMaybeList;

  let render = maybe =>
    Belt.Option.map(maybe, t =>
      Belt.List.reverse(t)
      |> String.concat(Type.separator)
      |> (x => Type.prefix ++ x)
    );
};

module Assignment = {
  type assignment = (string, string);
  type t = Belt.List.t(assignment);

  let add = (t, field, value) => addToMaybeList(t, (field, value));

  let render = maybe =>
    Belt.Option.map(maybe, t =>
      Belt.List.reverse(t)
      |. Belt.List.map(((field, value)) => {j|$field = $value|j})
      |. (x => String.concat("\n, ", x) |. (x => "SET\n  " ++ x))
    );
};

module Fields =
  Clause({
    let prefix = "  ";
    let separator = "\n, ";
  });

module From = {
  type t = string;
  let make = string : t => string;
  let render = maybe => Belt.Option.map(maybe, t => {j|FROM $t|j});
};

module GroupBy =
  Clause({
    let prefix = "GROUP BY\n  ";
    let separator = "\n, ";
  });

module Join =
  Clause({
    let prefix = "";
    let separator = "\n";
  });

module Limit = {
  type t =
    | Count(int)
    | Offset(int, int);

  let withCount = count => Count(count);

  let withOffset = (count, offset) => Offset(count, offset);

  let render = maybe =>
    Belt.Option.map(
      maybe,
      fun
      | Count(count) => {j|LIMIT $count|j}
      | Offset(count, offset) => {j|LIMIT $count OFFSET $offset|j},
    );
};

module type FlagType = {type flag; let toString: flag => string;};

module Modifier = (Flag: FlagType) => {
  type t = Belt.List.t(Flag.flag);

  let add = addToMaybeList;

  let render = maybe =>
    Belt.Option.map(maybe, t =>
      Belt.List.map(t, Flag.toString)
      |> Belt.List.reverse
      |> String.concat(" ")
      |> (x => " " ++ x)
    );
};

module OrderBy = {
  type ord = [ | `Asc(string) | `Desc(string)];
  type t = Belt.List.t(ord);

  let add = addToMaybeList;

  let toString =
    fun
    | `Asc(string) => {j|$string ASC|j}
    | `Desc(string) => {j|$string DESC|j};

  let render = maybe =>
    Belt.Option.map(maybe, t =>
      Belt.List.map(t, toString)
      |> Belt.List.reverse
      |> String.concat("\n, ")
      |> (x => "ORDER BY\n  " ++ x)
    );
};

module Table = {
  type t = string;
  let make = string : t => string;
  let render = maybe => Belt.Option.map(maybe, t => {j|  $t|j});
};

module Where =
  Clause({
    let prefix = "WHERE 1=1\n";
    let separator = "\n";
  });
