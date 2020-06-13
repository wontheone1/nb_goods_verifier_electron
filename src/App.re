module Ui = SemanticUi;

[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

[@bs.module "./renderer"]
external openOrderCSVFile: (Model.orderData => unit) => unit =
  "openOrderCSVFile";

[@bs.module "./renderer"]
external openEcountExcelFile: (Model.ecountData => unit) => unit =
  "openEcountExcelFile";

[@bs.module "./renderer"]
external saveOutputFile: array(Model.column) => unit = "saveOutputFile";

let initialState: Model.state = {
  orderData: {
    csvContents: [||],
    optionManagementCodeColumn: [||],
    orderArticleNameColumn: [||],
    articleOptionColumn: [||],
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
  outputContents: None,
};

let findIndexFromColumn = (item, col) => {
  let item = String.trim(item);
  if (item == "") {
    None;
  } else {
    Belt.Array.getIndexBy(col, value => item == String.trim(value));
  };
};

let reducer: (Model.state, Model.action) => Model.state =
  (state, action) => {
    switch (action) {
    | SetOrderData(orderData) => {...state, orderData}
    | SetEcountData(ecountData) => {...state, ecountData}
    | SetMatchResult(matchResult) => {...state, matchResult}
    | SetOutputContents(outputContents) => {...state, outputContents}
    };
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

  let match: unit => Model.matchResult =
    () => {
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
        let outputContents = DataProcesser.makeOutputContents(state);
        dispatch(SetOutputContents(Some(outputContents)));
      };
      None;
    },
    [|state.matchResult|],
  );

  React.useEffect1(
    () => {
      if (bothFileUploaded()) {
        Js.log("Output contents");
      };
      None;
    },
    [|state.outputContents|],
  );

  let panes: array(Ui.Tab.pane) = [|
    {
      menuItem: {j|매칭실패품목보기|j},
      render: () =>
        switch (state.outputContents) {
        | Some(outputContents) =>
          <Ui.Tab.Pane attached=false>
            <ResultTable
              tableContents={DataProcesser.filterOutputContentsByCheckResult(
                outputContents,
                Model.Fail,
              )}
            />
          </Ui.Tab.Pane>
        | None => ResultTable.noResult
        },
    },
    {
      menuItem: {j|모두보기|j},
      render: () =>
        switch (state.outputContents) {
        | Some(outputContents) =>
          <Ui.Tab.Pane attached=false>
            <ResultTable tableContents=outputContents />
          </Ui.Tab.Pane>
        | None => ResultTable.noResult
        },
    },
    {
      menuItem: {j|매칭성공품목보기|j},
      render: () =>
        switch (state.outputContents) {
        | Some(outputContents) =>
          <Ui.Tab.Pane attached=false>
            <ResultTable
              tableContents={DataProcesser.filterOutputContentsByCheckResult(
                outputContents,
                Model.Pass,
              )}
            />
          </Ui.Tab.Pane>
        | None => ResultTable.noResult
        },
    },
  |];

  <div>
    <p>
      {j|NB 검수 시스템에 오신 것을 환영합니다.|j}->React.string
    </p>
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
    <Ui.Tab menu={secondary: true, pointing: true} panes />
  </div>;
};
