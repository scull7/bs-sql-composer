open SqlComposer_component;

module Select = SqlComposer_select;

module Update = SqlComposer_update;

let updateFromSelect = select => {
  let table =
    select |. Select.getFrom |. From.raw |. Belt.Option.map(Table.make);

  Update.assemble(
    ~from=?table,
    ~join=?Select.getJoin(select),
    ~where=?Select.getWhere(select),
    ~orderBy=?Select.getOrderBy(select),
    ~limit=?Select.getLimit(select),
    (),
  );
};
