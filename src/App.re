[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

type column = array(string);
type intColumn = array(int);

type orderData = {
  optionManagementCodeColumn: column,
  orderArticleQtyColumn: column,
  orderArticlePayAmountIndexColumn: column,
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
};

[@bs.module "./renderer"]
external openOrderCSVFile: (orderData => unit) => unit = "openOrderCSVFile";

[@bs.module "./renderer"]
external openEcountExcelFile: (ecountData => unit) => unit =
  "openEcountExcelFile";

type action =
  | SetOrderData(orderData)
  | SetEcountData(ecountData)
  | SetMatchResult(matchResult);

let initialState = {
  orderData: {
    optionManagementCodeColumn: [||],
    orderArticleQtyColumn: [||],
    orderArticlePayAmountIndexColumn: [||],
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
        state.orderData.orderArticlePayAmountIndexColumn,
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
      Js.log(state.matchResult);
      None;
    },
    [|state.matchResult|],
  );

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
  </div>;
};
