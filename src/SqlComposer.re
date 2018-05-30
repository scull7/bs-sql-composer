module Select = {
  type t = {
    modifier: option(string),
    fields: list(string),
    from: list(string),
    join: list(string),
    where: list(string),
    group_by: list(string),
    order_by: list(string),
    limit: list(string),
  };

  let select = {
    modifier: None,
    fields: [],
    from: [],
    join: [],
    where: ["WHERE 1=1"],
    group_by: [],
    order_by: [],
    limit: [],
  };

  type flag = [
    | `Distinct
    | `HighPriority
    | `StraightJoin
    | `MySqlNoCache
    | `MySqlCalcFoundRows
  ];

  type ord = [ | `Asc(string) | `Desc(string)];

  let modifier = (query, flag) => {
    let modifier =
      switch (flag) {
      | `Distinct => "DISTINCT"
      | `HighPriority => "HIGH_PRIORITY"
      | `StraightJoin => "STRAIGHT_JOIN"
      | `MySqlNoCache => "SQL_NO_CACHE"
      | `MySqlCalcFoundRows => "SQL_CALC_FOUND_ROWS"
      };

    let setting =
      switch (query.modifier) {
      | Some(s) => String.concat(" ", [s, modifier])
      | None => modifier
      };

    {...query, modifier: Js.Option.some(setting)};
  };

  let field = (query, statement) => {
    let compiled =
      Belt_List.length(query.fields) > 0 ?
        {j|  , $statement|j} : {j|  $statement|j};
    {...query, fields: [compiled, ...query.fields]};
  };

  let from = (query, table) => {...query, from: [{j|FROM $table|j}]};

  let join = (query, statement) => {
    ...query,
    join: [statement, ...query.join],
  };

  let where = (query, statement) => {
    ...query,
    where: [statement, ...query.where],
  };

  let group_by = (query, statement) => {
    let group_by =
      Belt.List.length(query.group_by) < 1 ?
        [{j|  $statement|j}, "GROUP BY"] :
        [{j|, $statement|j}, ...query.group_by];
    {...query, group_by};
  };

  let order_by = (query, field) => {
    let clause =
      switch (field) {
      | `Asc(f) => {j|$f ASC|j}
      | `Desc(f) => {j|$f DESC|j}
      };
    let order_by =
      Belt.List.length(query.order_by) < 1 ?
        [{j|  $clause|j}, "ORDER BY"] : [{j|, $clause|j}, ...query.order_by];
    {...query, order_by};
  };

  let limit = (query, ~offset=?, ~row_count) => {
    let statement =
      switch (offset) {
      | Some(x) => {j|LIMIT $row_count OFFSET $x|j}
      | None => {j|LIMIT $row_count|j}
      };
    {...query, limit: [statement]};
  };

  let to_sql =
      ({modifier, fields, from, join, where, order_by, group_by, limit}) => {
    let clause =
      switch (modifier) {
      | Some(v) => {j|SELECT $v|j}
      | None => "SELECT"
      };
    let statements = [fields, from, join, where, order_by, group_by, limit];
    let reverseJoin = (sep, statements) =>
      String.concat(sep, Belt.List.reverse(statements));

    Belt.List.keep(statements, x => Belt.List.length(x) > 0)
    |> (x => [[clause], ...x])
    |> (x => Belt.List.map(x, reverseJoin("\n")))
    |> String.concat("\n");
  };
};
