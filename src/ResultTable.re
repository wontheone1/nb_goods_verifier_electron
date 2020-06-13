module Ui = SemanticUi;

type column = {
  name: string,
  rows: array(string),
};

let orderItemNameCol = {name: {j|주문상품명|j}, rows: [|"hello"|]};
let itemOptionCol = {name: {j|상품옵션|j}, rows: [|"hello"|]};
let matchingResultCol = {name: {j|검수결과|j}, rows: [|"hello"|]};
let articleMatchResultCol = {name: {j|품목체크|j}, rows: [|"hello"|]};
let qtyMatchResultCol = {name: {j|수량체크|j}, rows: [|"hello"|]};
let totalPriceMatchResultCol = {name: {j|단가체크|j}, rows: [|"hello"|]};
let resultTable = [|
  orderItemNameCol,
  itemOptionCol,
  matchingResultCol,
  articleMatchResultCol,
  qtyMatchResultCol,
  totalPriceMatchResultCol,
|];

[@react.component]
let make = (~tableContents: option(array(Model.column))) => {
  <Ui.Table>
    <Ui.Table.Header>
      <Ui.Table.Row>
        {switch (tableContents) {
         | None =>
           {j|csv파일과 excel파일을 업로드하십시오.|j}
           ->React.string
         | Some(tableContents) =>
           tableContents[0]
           ->Belt.Array.map(header => {
               Js.log(header);
               <Ui.Table.HeaderCell key=header>
                 header->React.string
               </Ui.Table.HeaderCell>;
             })
           ->React.array
         }}
      </Ui.Table.Row>
    </Ui.Table.Header>
  </Ui.Table>;
};
