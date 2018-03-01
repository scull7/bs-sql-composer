
module Query = struct
  type t = {
    select: string list;
    from: string list;
    join: string list;
    where: string list;
  }

  (* @TODO add order_by *)
  (* @TODO add group_by *)
  (* @TODO add limit *)

  let from table = {
    select = [];
    from = [ table; "FROM" ];
    join = [];
    where = [ "WHERE 1=1" ];
  }

  let select query statement = {
    query with select = statement :: query.select
  }

  let join query statement = {
    query with join = statement :: query.join
  }

  let where query statement = {
    query with where = statement :: query.where
  }

  module Pipe = struct
    let select statement query = select query statement
    let join statement query = join query statement
    let where statement query = where query statement
  end

  let to_sql { select; from; join; where } =
    let reverse_join sep statements =
      String.concat sep (Belt_List.reverse statements)
    in
      String.concat " " [
        (String.concat " " [ "SELECT"; (reverse_join ", " select) ]);
        (reverse_join " " from);
        (reverse_join " " join);
        (reverse_join " " where);
      ]
end
