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
