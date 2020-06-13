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
      <Ui.Table.Body>
        {Belt.Array.map(
           Belt.Array.range(1, Belt.Array.length(tableContents) - 1), i =>
           <Ui.Table.Row>
             {tableContents[i]
              ->Belt.Array.map(cellContent => {
                  <Ui.Table.Cell key=cellContent>
                    cellContent->React.string
                  </Ui.Table.Cell>
                })
              ->React.array}
           </Ui.Table.Row>
         )
         ->React.array}
      </Ui.Table.Body>
    </Ui.Table>
  };
};
