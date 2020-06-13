module Ui = SemanticUi;

[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

let panes: array(Ui.Tab.pane) = [|
  {
    menuItem: {j|매칭실패품목보기|j},
    render: () =>
      <Ui.Tab.Pane attached=false>
        <ResultTable tabName="success" />
      </Ui.Tab.Pane>,
  },
  {
    menuItem: {j|모두보기|j},
    render: () =>
      <Ui.Tab.Pane attached=false>
        <ResultTable tabName="all" />
      </Ui.Tab.Pane>,
  },
  {
    menuItem: {j|매칭성공품목보기|j},
    render: () =>
      <Ui.Tab.Pane attached=false>
        <ResultTable tabName="fail" />
      </Ui.Tab.Pane>,
  },
|];

type column = array(string);
type intColumn = array(int);

type orderData = {
  csvContents: array(column),
  optionManagementCodeColumn: column,
  orderArticleQtyColumn: column,
  orderArticlePayAmountColumn: column,
};

type ecountData = {
  articleCodeColumn: column,
  eaColumn: intColumn,
  totalColumn: intColumn,
};

type matchResult = {
  optionCodeToArticleCodeMatchResult: array(option(int)),
  orderQtyEaMatchResult: array(bool),
  payAmountTotalMatchResult: array(bool),
};

type state = {
  orderData,
  ecountData,
  matchResult,
  outputContents: array(column),
};

[@bs.module "./renderer"]
external openOrderCSVFile: (orderData => unit) => unit = "openOrderCSVFile";

[@bs.module "./renderer"]
external openEcountExcelFile: (ecountData => unit) => unit =
  "openEcountExcelFile";

[@bs.module "./renderer"]
external saveOutputFile: array(column) => unit = "saveOutputFile";

type action =
  | SetOrderData(orderData)
  | SetEcountData(ecountData)
  | SetMatchResult(matchResult)
  | SetOutputContents(array(column));

let initialState = {
  orderData: {
    csvContents: [||],
    optionManagementCodeColumn: [||],
    orderArticleQtyColumn: [||],
    orderArticlePayAmountColumn: [||],
  },
  ecountData: {
    articleCodeColumn: [||],
    eaColumn: [||],
    totalColumn: [||],
  },
  matchResult: {
    optionCodeToArticleCodeMatchResult: [||],
    orderQtyEaMatchResult: [||],
    payAmountTotalMatchResult: [||],
  },
  outputContents: [||],
};

let findIndexFromColumn = (item, col) => {
  let item = String.trim(item);
  if (item == "") {
    None;
  } else {
    Belt.Array.getIndexBy(col, value => item == String.trim(value));
  };
};

let reducer: (state, action) => state =
  (state, action) => {
    switch (action) {
    | SetOrderData(orderData) => {...state, orderData}
    | SetEcountData(ecountData) => {...state, ecountData}
    | SetMatchResult(matchResult) => {...state, matchResult}
    | SetOutputContents(outputContents) => {...state, outputContents}
    };
  };

let boolToPassFail: bool => string = v => if (v) {"PASS"} else {"FAIL"};

let optionToBool: option(int) => bool =
  v =>
    switch (v) {
    | Some(_) => true
    | None => false
    };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let optionCodeColumn = state.orderData.optionManagementCodeColumn;
  let articleCodeColumn = state.ecountData.articleCodeColumn;

  let bothFileUploaded = () =>
    Belt.Array.length(optionCodeColumn) != 0
    && Belt.Array.length(articleCodeColumn) != 0;

  let matchTwoColumnsUsingOptionCodeIdxAndArticleCodeIdx =
      (
        optionCodeToArticleCodeMatchResult,
        columnFromOrderData,
        columnFromEcountData,
      ) => {
    Belt.Array.mapWithIndex(
      optionCodeToArticleCodeMatchResult,
      (indexOptionCodeColumn, indexArticleCodeColumn) =>
      switch (indexArticleCodeColumn) {
      | Some(indexArticleCodeColumn) =>
        columnFromOrderData[indexOptionCodeColumn]->int_of_string
        == columnFromEcountData[indexArticleCodeColumn]
      | None => false
      }
    );
  };

  let match = () => {
    let optionCodeToArticleCodeMatchResult =
      Belt.Array.map(optionCodeColumn, (optionCode: string) =>
        findIndexFromColumn(optionCode, articleCodeColumn)
      );

    let orderQtyEaMatchResult =
      matchTwoColumnsUsingOptionCodeIdxAndArticleCodeIdx(
        optionCodeToArticleCodeMatchResult,
        state.orderData.orderArticleQtyColumn,
        state.ecountData.eaColumn,
      );

    let payAmountTotalMatchResult =
      matchTwoColumnsUsingOptionCodeIdxAndArticleCodeIdx(
        optionCodeToArticleCodeMatchResult,
        state.orderData.orderArticlePayAmountColumn,
        state.ecountData.totalColumn,
      );

    {
      optionCodeToArticleCodeMatchResult,
      orderQtyEaMatchResult,
      payAmountTotalMatchResult,
    };
  };

  React.useEffect1(
    () => {
      Js.log(state.orderData);
      if (bothFileUploaded()) {
        dispatch(SetMatchResult(match()));
      };
      None;
    },
    [|state.orderData|],
  );

  React.useEffect1(
    () => {
      Js.log(state.ecountData);
      if (bothFileUploaded()) {
        dispatch(SetMatchResult(match()));
      };
      None;
    },
    [|state.ecountData|],
  );

  React.useEffect1(
    () => {
      if (bothFileUploaded()) {
        Js.log(state.matchResult);
        let outputContents = {
          Belt.Array.mapWithIndex(state.orderData.csvContents, (i, row) =>
            if (i == 0) {
              Belt.Array.concat(
                row,
                [|
                  {j|검수결과|j},
                  {j|품목체크|j},
                  {j|수량체크|j},
                  {j|단가체크|j},
                |],
              );
            } else {
              let idx = i - 1;
              let articleCodeMatchResult =
                state.matchResult.optionCodeToArticleCodeMatchResult[idx]
                ->optionToBool;
              let qtyCheckResult = state.matchResult.orderQtyEaMatchResult[idx];
              let totalCheckResult = state.matchResult.
                                       payAmountTotalMatchResult[idx];
              Belt.Array.concat(
                row,
                [|
                  boolToPassFail(
                    qtyCheckResult
                    && totalCheckResult
                    && articleCodeMatchResult,
                  ),
                  boolToPassFail(articleCodeMatchResult),
                  boolToPassFail(qtyCheckResult),
                  boolToPassFail(totalCheckResult),
                |],
              );
            }
          );
        };
        dispatch(SetOutputContents(outputContents));
      };
      None;
    },
    [|state.matchResult|],
  );

  React.useEffect1(
    () => {
      if (bothFileUploaded()) {
        Js.log("Output contents");
        Js.log(state.outputContents);
        saveOutputFile(state.outputContents);
      };
      None;
    },
    [|state.outputContents|],
  );

  <div>
    <p>
      {j|NB 검수 시스템에 오신 것을 환영합니다.|j}->React.string
    </p>
    <Ui.Tab menu={secondary: true, pointing: true} panes />
    <button
      onClick={_ =>
        openOrderCSVFile(orderData => {dispatch(SetOrderData(orderData))})
      }>
      {j|쇼핑몰 주문 csv 파일 선택|j}->React.string
    </button>
    <button
      onClick={_ =>
        openEcountExcelFile(ecountData =>
          dispatch(SetEcountData(ecountData))
        )
      }>
      {j|이카운트 주문입력 xlsx 파일 선택|j}->React.string
    </button>
  </div>;
};
