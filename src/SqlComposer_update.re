open SqlComposer_component;

module Select = SqlComposer_select;

module Modifier =
  Modifier({
    type flag = [ | `LowPriority | `Ignore];

    let toString =
      fun
      | `LowPriority => "LOW_PRIORITY"
      | `Ignore => "IGNORE";
  });

type t = {
  modifier: option(Modifier.t),
  from: option(Table.t),
  join: option(Join.t),
  set: option(Assignment.t),
  where: option(Where.t),
  orderBy: option(OrderBy.t),
  limit: option(Limit.t),
};

let make = () => {
  modifier: None,
  from: None,
  join: None,
  set: None,
  where: None,
  orderBy: None,
  limit: None,
};

let assemble =
    (~modifier=?, ~from=?, ~join=?, ~set=?, ~where=?, ~orderBy=?, ~limit=?, _) => {
  modifier,
  from,
  join,
  set,
  where,
  orderBy,
  limit,
};

let modifier = (query, flag) => {
  ...query,
  modifier: Modifier.add(query.modifier, flag),
};

let from = (query, table) => {...query, from: Some(Table.make(table))};

let join = (query, join) => {...query, join: Join.add(query.join, join)};

let set = (query, field, value) => {
  ...query,
  set: Assignment.add(query.set, field, value),
};

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

let toSql = ({modifier, from, join, set, where, orderBy, limit}) => {
  let statements = [
    Modifier.render(modifier),
    Table.render(from),
    Join.render(join),
    Assignment.render(set),
    Where.render(where),
    OrderBy.render(orderBy),
    Limit.render(limit),
  ];

  let prefix = Belt.Option.mapWithDefault(modifier, "UPDATE\n", _ => "UPDATE");

  Belt.List.keep(statements, Belt.Option.isSome)
  |. Belt.List.map(Belt.Option.getExn)
  |. (x => String.concat("\n", x) |. (x => prefix ++ x));
};
