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
   *     make()
   *     |. field("*")
   *     |. from("test")
   *     |. where("AND test.foo = ?")
   *     |. toSql
   *   );
   * Js.log2("SQL: ", sql);
   *````
   */;
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
};

module Update: {
  /**
   * # SqlComposer.Update
   *
   * Generate composable SQL Update statements and fragments.
   *
   * Here is a basic example:
   * ```reason
   * let sql =
   *   SqlComposer.Update.(
   *     make()
   *     |. from("test")
   *     |. set("foo", "?")
   *     |. set("bar", "?")
   *     |. where({|AND moo = "cow"|})
   *     |. toSql
   *   );
   * Js.log2("SQL: ", sql);
   * ```
   */;
  type t;

  let make: unit => t;

  let modifier: (t, [ | `Ignore | `LowPriority]) => t;

  let from: (t, string) => t;

  let join: (t, string) => t;

  let set: (t, string, string) => t;

  let where: (t, string) => t;

  let orderBy: (t, [ | `Asc(string) | `Desc(string)]) => t;

  let limit: (t, ~offset: int=?, ~row_count: int) => t;

  let toSql: t => string;
};
