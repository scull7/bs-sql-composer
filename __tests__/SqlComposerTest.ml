open Jest

let () =

describe "Test SqlComposer Interface" (fun () ->
  test "A simple query" (fun () ->
    let expected = "SELECT\n  *\nFROM test\nWHERE 1=1" in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "A query with a join" (fun () ->
    let expected =
      "SELECT\n  *\nFROM test\nJOIN foo\nWHERE 1=1"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> join "JOIN foo"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "A query with a where clause" (fun () ->
    let expected =
      "SELECT\n  *\nFROM test\nWHERE 1=1\nAND test.foo = ?"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> where "AND test.foo = ?"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding to a base query" (fun () ->
    let expected =
      "SELECT\n  foo\n  , bar\nFROM test\nWHERE 1=1\nAND test.foo = ?"
    in
    let base_query = SqlComposer.Select.(
      select
      |> from "test"
      |> field "foo"
      |> field "bar"
    )
    in
    let actual = SqlComposer.Select.(
      where "AND test.foo = ?" base_query
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Field alias" (fun () ->
    let expected =
      "SELECT\n  foo AS bar\nFROM test\nWHERE 1=1"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "foo AS bar"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Limit" (fun () ->
    let expected =
      "SELECT\n  foo AS bar\nFROM test\nWHERE 1=1\nLIMIT 10"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "foo AS bar"
      |> limit ~row_count:10
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Limit with offset" (fun () ->
    let expected =
      "SELECT\n  foo AS bar\nFROM test\nWHERE 1=1\nLIMIT 10 OFFSET 2"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "foo AS bar"
      |> limit ~row_count:10 ~offset:2
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Order By DESC" (fun () ->
    let expected =
      "SELECT
  foo AS bar
FROM test
WHERE 1=1
ORDER BY
  foo DESC
LIMIT 10 OFFSET 2"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> order_by (`Desc "foo")
      |> field "foo AS bar"
      |> limit ~row_count:10 ~offset:2
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Order By ASC" (fun () ->
    let expected =
      "SELECT
  foo AS bar
FROM test
WHERE 1=1
ORDER BY
  foo ASC
LIMIT 10 OFFSET 2"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> order_by (`Asc "foo")
      |> field "foo AS bar"
      |> limit ~row_count:10 ~offset:2
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Order By 2 fields" (fun () ->
    let expected =
      "SELECT
  foo AS bar
FROM test
WHERE 1=1
ORDER BY
  foo ASC
, bar DESC
LIMIT 10 OFFSET 2"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> order_by (`Asc "foo")
      |> order_by (`Desc "bar")
      |> field "foo AS bar"
      |> limit ~row_count:10 ~offset:2
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding a distinct modifier" (fun () ->
    let expected = "SELECT DISTINCT\n  *\nFROM test\nWHERE 1=1" in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> modifier `Distinct
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding a STRAIGHT_JOIN modifier" (fun () ->
    let expected = "SELECT STRAIGHT_JOIN\n  *\nFROM test\nWHERE 1=1" in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> modifier `StraightJoin
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding a SQL_NO_CACHE modifier" (fun () ->
    let expected = "SELECT SQL_NO_CACHE\n  *\nFROM test\nWHERE 1=1" in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> modifier `MySqlNoCache
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding a SQL_CALC_FOUND_ROWS modifier" (fun () ->
    let expected = "SELECT SQL_CALC_FOUND_ROWS\n  *\nFROM test\nWHERE 1=1" in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> field "*"
      |> modifier `MySqlCalcFoundRows
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding a distinct and high priority modifier" (fun () ->
    let expected = "SELECT DISTINCT HIGH_PRIORITY\n  *\nFROM test\nWHERE 1=1" in
    let actual = SqlComposer.Select.(
      select
      |> modifier `Distinct
      |> from "test"
      |> field "*"
      |> modifier `HighPriority
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Group By" (fun () ->
    let expected =
"SELECT DISTINCT
  foo
FROM test
WHERE 1=1
GROUP BY
  foo"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> group_by "foo"
      |> field "foo"
      |> modifier `Distinct
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Group By multiple" (fun () ->
    let expected =
"SELECT DISTINCT
  foo
FROM test
WHERE 1=1
GROUP BY
  foo
, bar"
    in
    let actual = SqlComposer.Select.(
      select
      |> from "test"
      |> group_by "foo"
      |> field "foo"
      |> modifier `Distinct
      |> group_by "bar"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
);
