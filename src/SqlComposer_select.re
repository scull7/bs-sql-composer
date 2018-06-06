open SqlComposer_component;

module Modifier =
  Modifier({
    type flag = [
      | `Distinct
      | `HighPriority
      | `StraightJoin
      | `MySqlNoCache
      | `MySqlCalcFoundRows
    ];
    let toString =
      fun
      | `Distinct => "DISTINCT"
      | `HighPriority => "HIGH_PRIORITY"
      | `StraightJoin => "STRAIGHT_JOIN"
      | `MySqlNoCache => "SQL_NO_CACHE"
      | `MySqlCalcFoundRows => "SQL_CALC_FOUND_ROWS";
  });

type t = {
  modifier: option(Modifier.t),
  fields: option(Fields.t),
  from: option(From.t),
  join: option(Join.t),
  where: option(Where.t),
  groupBy: option(GroupBy.t),
  orderBy: option(OrderBy.t),
  limit: option(Limit.t),
};

let make = () => {
  modifier: None,
  fields: None,
  from: None,
  join: None,
  where: None,
  groupBy: None,
  orderBy: None,
  limit: None,
};

let modifier = (query, flag) => {
  ...query,
  modifier: Modifier.add(query.modifier, flag),
};

let field = (query, field) => {
  ...query,
  fields: Fields.add(query.fields, field),
};

let from = (query, table) => {...query, from: Some(From.make(table))};

let join = (query, join) => {...query, join: Join.add(query.join, join)};

let where = (query, where) => {
  ...query,
  where: Where.add(query.where, where),
};

let groupBy = (query, group) => {
  ...query,
  groupBy: GroupBy.add(query.groupBy, group),
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

let toSql = ({modifier, fields, from, join, where, orderBy, groupBy, limit}) => {
  let statements = [
    Modifier.render(modifier),
    Fields.render(fields),
    From.render(from),
    Join.render(join),
    Where.render(where),
    OrderBy.render(orderBy),
    GroupBy.render(groupBy),
    Limit.render(limit),
  ];
  let prefix = Belt.Option.mapWithDefault(modifier, "SELECT\n", _ => "SELECT");

  Belt.List.keep(statements, Belt.Option.isSome)
  |. Belt.List.map(Belt.Option.getExn)
  |. (x => String.concat("\n", x) |. (x => prefix ++ x));
};
