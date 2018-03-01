open Jest

let () =

describe "Test SqlComposer Interface" (fun () ->
  test "A simple query" (fun () ->
    let expected = "SELECT * FROM test  WHERE 1=1" in
    let actual = SqlComposer.Query.(
      from "test"
      |> Pipe.select "*"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "A query with a join" (fun () ->
    let expected =
      "SELECT * FROM test JOIN foo WHERE 1=1"
    in
    let actual = SqlComposer.Query.(
      from "test"
      |> Pipe.select "*"
      |> Pipe.join "JOIN foo"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "A query with a where clause" (fun () ->
    let expected =
      "SELECT * FROM test  WHERE 1=1 AND test.foo = ?"
    in
    let actual = SqlComposer.Query.(
      from "test"
      |> Pipe.select "*"
      |> Pipe.where "AND test.foo = ?"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  );
  test "Adding to a base query" (fun () ->
    let expected =
      "SELECT foo, bar FROM test  WHERE 1=1 AND test.foo = ?"
    in
    let base_query = SqlComposer.Query.(
      from "test"
      |> Pipe.select "foo"
      |> Pipe.select "bar"
    )
    in
    let actual = SqlComposer.Query.(
      where base_query "AND test.foo = ?"
      |> to_sql
    )
    in
    Expect.expect actual |> Expect.toBe expected
  )
);
