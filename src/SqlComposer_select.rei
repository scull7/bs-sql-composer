type t;

let make: unit => t;

let modifier:
  (
    t,
    [
      | `Distinct
      | `HighPriority
      | `StraightJoin
      | `MySqlNoCache
      | `MySqlCalcFoundRows
    ]
  ) =>
  t;

let field: (t, string) => t;

let from: (t, string) => t;

let join: (t, string) => t;

let where: (t, string) => t;

let groupBy: (t, string) => t;

let orderBy: (t, [ | `Asc(string) | `Desc(string)]) => t;

let limit: (t, ~offset: int=?, ~row_count: int) => t;

let toSql: t => string;

let getFrom: t => option(SqlComposer_component.From.t);

let getWhere: t => option(SqlComposer_component.Where.t);

let getJoin: t => option(SqlComposer_component.Join.t);

let getOrderBy: t => option(SqlComposer_component.OrderBy.t);

let getLimit: t => option(SqlComposer_component.Limit.t);
