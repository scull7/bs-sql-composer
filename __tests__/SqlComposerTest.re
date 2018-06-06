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
    SqlComposer.Select.(make() |. from("test") |. field("*") |. toSql)
  );

  genFileTest("A query with a join", "select-join", () =>
    SqlComposer.Select.(
      make() |. from("test") |. field("*") |. join("JOIN foo") |. toSql
    )
  );

  genFileTest("A query with a where clause", "select-where", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. field("*")
      |. where("AND test.foo = ?")
      |. toSql
    )
  );

  genFileTest("Adding to a base query", "composition-basic", () => {
    let baseQuery =
      SqlComposer.Select.(
        make() |. from("test") |. field("foo") |. field("bar")
      );

    SqlComposer.Select.(baseQuery |. where("AND test.foo = ?") |. toSql);
  });

  genFileTest("Field alias", "select-field-alias", () =>
    SqlComposer.Select.(
      make() |. from("test") |. field("foo AS bar") |. toSql
    )
  );

  genFileTest("Limit", "select-limit", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=?None)
      |. toSql
    )
  );

  genFileTest("Limit with offset", "select-limit-offset", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. toSql
    )
  );

  genFileTest("Order BY ASC", "order-by-asc", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. orderBy(`Asc("foo"))
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. toSql
    )
  );
  genFileTest("Order BY DESC", "order-by-desc", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. orderBy(`Desc("foo"))
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. toSql
    )
  );
  genFileTest("Order By 2 fields", "order-by-2-fields", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. orderBy(`Asc("foo"))
      |. orderBy(`Desc("bar"))
      |. field("foo AS bar")
      |. limit(~row_count=10, ~offset=2)
      |. toSql
    )
  );

  genFileTest("Group By", "group-by", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. groupBy("foo")
      |. field("foo")
      |. modifier(`Distinct)
      |. toSql
    )
  );

  genFileTest("Group By multiple", "group-by-multiple", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. groupBy("foo")
      |. field("foo")
      |. modifier(`Distinct)
      |. groupBy("bar")
      |. toSql
    )
  );

  genFileTest("Adding a DISTINCT modifier", "modifier-distinct", () =>
    SqlComposer.Select.(
      make() |. from("test") |. modifier(`Distinct) |. field("*") |. toSql
    )
  );

  genFileTest("Adding a STRAIGHT_JOIN modifier", "modifier-straight-join", () =>
    SqlComposer.Select.(
      make()
      |. modifier(`StraightJoin)
      |. from("test")
      |. field("*")
      |. toSql
    )
  );

  genFileTest("Adding a SQL_NO_CACHE modifier", "modifier-no-cache", () =>
    SqlComposer.Select.(
      make()
      |. modifier(`MySqlNoCache)
      |. from("test")
      |. field("*")
      |. toSql
    )
  );

  genFileTest(
    "Adding a SQL_CALC_FOUND_ROWS modifier", "modifier-found-rows", () =>
    SqlComposer.Select.(
      make()
      |. from("test")
      |. field("*")
      |. modifier(`MySqlCalcFoundRows)
      |. toSql
    )
  );

  genFileTest(
    "Adding a DISTINCT AND HIGH_PRIORITY modifier",
    "modifier-distinct-priority",
    () =>
    SqlComposer.Select.(
      make()
      |. modifier(`Distinct)
      |. from("test")
      |. field("*")
      |. modifier(`HighPriority)
      |. toSql
    )
  );
});

describe("Update Interface", () => {
  genFileTest("An update query", "update-where", () =>
    SqlComposer.Update.(
      make()
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("An ordered ASC update query", "update-order-asc", () =>
    SqlComposer.Update.(
      make()
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("An ordered DESC update query", "update-order-desc", () =>
    SqlComposer.Update.(
      make()
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("A multiple ordered update query", "update-order-mult", () =>
    SqlComposer.Update.(
      make()
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("A low priority update query", "update-low-priority", () =>
    SqlComposer.Update.(
      make()
      |. modifier(`LowPriority)
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("An update query ignore errors", "update-ignore", () =>
    SqlComposer.Update.(
      make()
      |. modifier(`Ignore)
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("All modifiers update query", "update-all-modifiers", () =>
    SqlComposer.Update.(
      make()
      |. modifier(`Ignore)
      |. modifier(`LowPriority)
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("update query with a limit", "update-limit", () =>
    SqlComposer.Update.(
      make()
      |. limit(~row_count=10, ~offset=?None)
      |. modifier(`Ignore)
      |. modifier(`LowPriority)
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("update query with an offset", "update-offset", () =>
    SqlComposer.Update.(
      make()
      |. limit(~row_count=10, ~offset=9)
      |. modifier(`Ignore)
      |. modifier(`LowPriority)
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );

  genFileTest("update query with a join", "update-join", () =>
    SqlComposer.Update.(
      make()
      |. limit(~row_count=10, ~offset=?None)
      |. modifier(`Ignore)
      |. join("JOIN animal ON test.animal_id = animal.id")
      |. modifier(`LowPriority)
      |. from("test")
      |. set("foo", "?")
      |. set("bar", "?")
      |. orderBy(`Desc("bar"))
      |. orderBy(`Asc("foo"))
      |. where({|AND moo = "cow"|})
      |. toSql
    )
  );
});

describe("Conversion Inteface", () =>
  genFileTest("Select to Update", "conversion-select-to-update", () => {
    let select =
      SqlComposer.Select.(
        make()
        |. from("test")
        |. join("JOIN animal ON test.animal_id = animal.id")
        |. where({|AND cow = "moo"|})
        |. where({|AND dog = "bark"|})
        |. orderBy(`Asc("cat"))
        |. limit(~offset=5, ~row_count=10)
      );
    SqlComposer.Conversion.updateFromSelect(select)
    |. SqlComposer.Update.set("foo", "?")
    |. SqlComposer.Update.toSql;
  })
);
