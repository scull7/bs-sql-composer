open Jest;

[@bs.module "fs"]
external readFileSync :
  (~name: string, [@bs.string] [ | `utf8 | [@bs.as "ascii"] `useAscii]) =>
  string =
  "";

let dirname =
  switch ([%bs.node __dirname]) {
  | Some(x) => x
  | None => failwith("Could not determine file location")
  };

let loadSql = name => {
  let filename = {j|$dirname/assets/$name.sql|j};
  readFileSync(~name=filename, `utf8) |. String.trim;
};

let genFileTest = (name, file, sqlGenerator) =>
  test(
    name,
    () => {
      let expected = loadSql(file);
      let actual = sqlGenerator();

      Expect.expect(actual) |> Expect.toBe(expected);
    },
  );

describe("Select Interface", () => {
  genFileTest("A basic query", "select-basic", () =>
    SqlComposer.Select.(select |. from("test") |. field("*") |. to_sql)
  );

  genFileTest("A query with a join", "select-join", () =>
    SqlComposer.Select.(
      select |. from("test") |. field("*") |. join("JOIN foo") |. to_sql
    )
  );

  genFileTest("A query with a where clause", "select-where", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. field("*")
      |. where("AND test.foo = ?")
      |. to_sql
    )
  );

  genFileTest("Adding to a base query", "composition-basic", () => {
    let baseQuery =
      SqlComposer.Select.(
        select |. from("test") |. field("foo") |. field("bar")
      );

    SqlComposer.Select.(baseQuery |. where("AND test.foo = ?") |. to_sql);
  });

  genFileTest("Field alias", "select-field-alias", () =>
    SqlComposer.Select.(
      select |. from("test") |. field("foo AS bar") |. to_sql
    )
  );

  genFileTest("Limit", "select-limit", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=?None)
      |. to_sql
    )
  );

  genFileTest("Limit with offset", "select-limit-offset", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. to_sql
    )
  );

  genFileTest("Order BY ASC", "order-by-asc", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. order_by(`Asc("foo"))
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. to_sql
    )
  );
  genFileTest("Order BY DESC", "order-by-desc", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. order_by(`Desc("foo"))
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. to_sql
    )
  );
  genFileTest("Order By 2 fields", "order-by-2-fields", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. order_by(`Asc("foo"))
      |. order_by(`Desc("bar"))
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. to_sql
    )
  );

  genFileTest("Group By", "group-by", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. group_by("foo")
      |. field("foo")
      |. modifier(`Distinct)
      |. to_sql
    )
  );

  genFileTest("Group By multiple", "group-by-multiple", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. group_by("foo")
      |. field("foo")
      |. modifier(`Distinct)
      |. group_by("bar")
      |. to_sql
    )
  );

  genFileTest("Adding a DISTINCT modifier", "modifier-distinct", () =>
    SqlComposer.Select.(
      select |. from("test") |. modifier(`Distinct) |. field("*") |. to_sql
    )
  );

  genFileTest("Adding a STRAIGHT_JOIN modifier", "modifier-straight-join", () =>
    SqlComposer.Select.(
      select
      |. modifier(`StraightJoin)
      |. from("test")
      |. field("*")
      |. to_sql
    )
  );

  genFileTest("Adding a SQL_NO_CACHE modifier", "modifier-no-cache", () =>
    SqlComposer.Select.(
      select
      |. modifier(`MySqlNoCache)
      |. from("test")
      |. field("*")
      |. to_sql
    )
  );

  genFileTest(
    "Adding a SQL_CALC_FOUND_ROWS modifier", "modifier-found-rows", () =>
    SqlComposer.Select.(
      select
      |. from("test")
      |. field("*")
      |. modifier(`MySqlCalcFoundRows)
      |. to_sql
    )
  );

  genFileTest(
    "Adding a DISTINCT AND HIGH_PRIORITY modifier",
    "modifier-distinct-priority",
    () =>
    SqlComposer.Select.(
      select
      |. modifier(`Distinct)
      |. from("test")
      |. field("*")
      |. modifier(`HighPriority)
      |. to_sql
    )
  );
});
