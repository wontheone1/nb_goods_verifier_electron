[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

type column = array(string);

type orderData = {
  optionManagementCodeColumn: column,
  orderArticleQtyColumn: column,
  orderArticlePayAmountIndexColumn: column,
};

type ecountData = {
  articleCodeColumn: column,
  eaColumn: column,
  totalColumn: column,
};

type matchResult = {
  optionCodeToArticleCodeMatchResult: array((int, option(int))),
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

  let match = () =>
    Belt.Array.mapWithIndex(optionCodeColumn, (i: int, optionCode: string) =>
      (i, findIndexFromColumn(optionCode, articleCodeColumn))
    );

  React.useEffect1(
    () => {
      Js.log(state.orderData);
      if (bothFileUploaded()) {
        dispatch(
          SetMatchResult({optionCodeToArticleCodeMatchResult: match()}),
        );
      };
      None;
    },
    [|state.orderData|],
  );

  React.useEffect1(
    () => {
      Js.log(state.ecountData);
      if (bothFileUploaded()) {
        dispatch(
          SetMatchResult({optionCodeToArticleCodeMatchResult: match()}),
        );
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
