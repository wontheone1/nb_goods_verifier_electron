module Ui = SemanticUi;

[@react.component]
let make = (~tableContents: option(array(Model.column))) => {
  switch (tableContents) {
  | None =>
    {j|csv파일과 excel파일을 업로드하십시오.|j}->React.string
  | Some(tableContents) =>
    <Ui.Table>
      <Ui.Table.Header>
        <Ui.Table.Row>
          {tableContents[0]
           ->Belt.Array.map(header => {
               Js.log(header);
               <Ui.Table.HeaderCell key=header>
                 header->React.string
               </Ui.Table.HeaderCell>;
             })
           ->React.array}
        </Ui.Table.Row>
      </Ui.Table.Header>
    </Ui.Table>
  };
};
