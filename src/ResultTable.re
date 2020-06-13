module Ui = SemanticUi;

let noResult =
  {j|csv파일과 excel파일을 업로드하십시오.|j}->React.string;

[@react.component]
let make = (~tableContents: array(Model.column)) => {
  <Ui.Table>
    <Ui.Table.Header>
      <Ui.Table.Row>
        {tableContents[0]
         ->Belt.Array.map(header => {
             <Ui.Table.HeaderCell key=header>
               header->React.string
             </Ui.Table.HeaderCell>
           })
         ->React.array}
      </Ui.Table.Row>
    </Ui.Table.Header>
    <Ui.Table.Body>
      {Belt.Array.map(
         Belt.Array.range(1, Belt.Array.length(tableContents) - 1), i => {
         <Ui.Table.Row key={string_of_int(i)}>
           {tableContents[i]
            ->Belt.Array.mapWithIndex((i, cellContent) => {
                <Ui.Table.Cell key={string_of_int(i)}>
                  cellContent->React.string
                </Ui.Table.Cell>
              })
            ->React.array}
         </Ui.Table.Row>
       })
       ->React.array}
    </Ui.Table.Body>
  </Ui.Table>;
};
