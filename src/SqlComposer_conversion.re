open SqlComposer_component;

module Delete = SqlComposer_delete;

module Select = SqlComposer_select;

module Update = SqlComposer_update;

let deleteFromSelect = select =>
  SqlComposer_delete.assemble(
    ~from=?Select.getFrom(select),
    ~where=?Select.getWhere(select),
    ~orderBy=?Select.getOrderBy(select),
    ~limit=?Select.getLimit(select),
    (),
  );

let updateFromSelect = select => {
  let table =
    select |. Select.getFrom |. From.raw |. Belt.Option.map(Table.make);

  SqlComposer_update.assemble(
    ~from=?table,
    ~join=?Select.getJoin(select),
    ~where=?Select.getWhere(select),
    ~orderBy=?Select.getOrderBy(select),
    ~limit=?Select.getLimit(select),
    (),
  );
};
