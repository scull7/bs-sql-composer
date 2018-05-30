/**
 * # SqlComposer
 *
 */;

module Select: {
  /**
   *  # SqlComposer.Select
   *
   * Generate composable SQL Select queries and query fragments.
   *
   * Here is a basic example:
   * ```reason
   * let sql =
   *   SqlComposer.Select.(
   *     select
   *     |. field("*")
   *     |. from("test")
   *     |. where("AND test.foo = ?")
   *     |. to_sql
   *   );
   * Js.log2("SQL: ", sql);
   *````
   */;
  type t;

  type flag = [
    | `Distinct
    | `HighPriority
    | `StraightJoin
    | `MySqlNoCache
    | `MySqlCalcFoundRows
  ];

  type ord = [ | `Asc(string) | `Desc(string)];

  let select: t;

  let modifier: (t, flag) => t;

  let field: (t, string) => t;

  let from: (t, string) => t;

  let join: (t, string) => t;

  let where: (t, string) => t;

  let group_by: (t, string) => t;

  let order_by: (t, ord) => t;

  let limit: (t, ~offset: int=?, ~row_count: int) => t;

  let to_sql: t => string;
};
