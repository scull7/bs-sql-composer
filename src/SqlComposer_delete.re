open SqlComposer_component;

module Modifier =
  Modifier({
    type flag = [ | `LowPriority | `Quick | `Ignore];

    let toString =
      fun
      | `LowPriority => "LOW_PRIORITY"
      | `Quick => "QUICK"
      | `Ignore => "IGNORE";
  });

type t = {
  modifier: option(Modifier.t),
  from: option(From.t),
  where: option(Where.t),
  orderBy: option(OrderBy.t),
  limit: option(Limit.t),
};

let make = () => {
  modifier: None,
  from: None,
  where: None,
  orderBy: None,
  limit: None,
};

let assemble = (~modifier=?, ~from=?, ~where=?, ~orderBy=?, ~limit=?, _) => {
  modifier,
  from,
  where,
  orderBy,
  limit,
};

let modifier = (query, flag) => {
  ...query,
  modifier: Modifier.add(query.modifier, flag),
};

let from = (query, table) => {...query, from: Some(From.make(table))};

let where = (query, where) => {
  ...query,
  where: Where.add(query.where, where),
};

let orderBy = (query, order) => {
  ...query,
  orderBy: OrderBy.add(query.orderBy, order),
};

let limit = (query, ~offset=?, ~row_count) => {
  ...query,
  limit:
    switch (offset) {
    | Some(o) => Some(Limit.withOffset(row_count, o))
    | None => Some(Limit.withCount(row_count))
    },
};

let toSql = ({modifier, from, where, orderBy, limit}) => {
  let statements = [
    Modifier.render(modifier),
    From.render(from),
    Where.render(where),
    OrderBy.render(orderBy),
    Limit.render(limit),
  ];

  let prefix = Belt.Option.mapWithDefault(modifier, "DELETE\n", _ => "DELETE");

  Belt.List.keep(statements, Belt.Option.isSome)
  |. Belt.List.map(Belt.Option.getExn)
  |. (x => String.concat("\n", x) |. (x => prefix ++ x));
};
