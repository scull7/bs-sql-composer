
module Select = struct
  type t = {
    modifier: string option;
    fields: string list;
    from: string list;
    join: string list;
    where: string list;
    group_by: string list;
    order_by: string list;
    limit: string list;
  }

  let select = {
    modifier = None;
    fields = [];
    from = [];
    join = [];
    where = [ "WHERE 1=1" ];
    group_by = [];
    order_by = [];
    limit = [];
  }

  let modifier flag query =
    let modifier = match flag with
      | `Distinct -> "DISTINCT"
      | `HighPriority -> "HIGH_PRIORITY"
      | `StraightJoin -> "STRAIGHT_JOIN"
      | `MySqlNoCache -> "SQL_NO_CACHE"
      | `MySqlCalcFoundRows -> "SQL_CALC_FOUND_ROWS"
    in
    let setting = match query.modifier with
      | Some s -> String.concat " " [ s; modifier ]
      | None -> modifier
    in
    { query with modifier = Js.Option.some setting }

  let field statement query =
    let compiled = if Belt_List.length query.fields > 0
      then {j|  , $statement|j}
      else {j|  $statement|j}
    in
    { query with fields = compiled :: query.fields}

  let from table query = {
    query with from = [ {j|FROM $table|j}]
  }

  let join statement query = {
    query with join = statement :: query.join
  }

  let where statement query = {
    query with where = statement :: query.where
  }

  let group_by statement query =
    let group_by = if Belt_List.length query.group_by < 1
      then [ {j|  $statement|j}; "GROUP BY" ]
      else {j|, $statement|j} :: query.group_by
    in
    { query with group_by = group_by }

  let order_by field query =
    let clause = match field with
      | `Asc f -> {j|$f ASC|j}
      | `Desc f -> {j|$f DESC|j}
    in
    let order_by = if Belt_List.length query.order_by < 1
      then [ {j|  $clause|j}; "ORDER BY" ]
      else {j|, $clause|j} :: query.order_by
    in
    { query with order_by = order_by }

  let limit ?offset ~row_count query =
    let statement = match offset with
      | Some x -> {j|LIMIT $row_count OFFSET $x|j}
      | None -> {j|LIMIT $row_count|j}
    in
   { query with limit = [ statement ] }

  let to_sql { modifier; fields; from; join; where; order_by; group_by; limit } =
    let clause = match modifier with
      | Some v -> {j|SELECT $v|j}
      | None -> "SELECT"
    in
    let statements = [ fields; from; join; where; order_by; group_by; limit ] in
    let reverse_join sep statements =
      String.concat sep (Belt_List.reverse statements)
    in
    Belt_List.keep statements (fun x -> Belt_List.length x > 0)
    |> (fun x -> [ clause ] :: x)
    |> (fun x -> Belt_List.map x (reverse_join "\n"))
    |> String.concat "\n"
end
