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

type state = {
  orderData,
  ecountData,
};

[@bs.module "./renderer"]
external openOrderCSVFile: (orderData => unit) => unit = "openOrderCSVFile";

[@bs.module "./renderer"]
external openEcountExcelFile: (ecountData => unit) => unit =
  "openEcountExcelFile";

type action =
  | SetOrderData(orderData)
  | SetEcountData(ecountData);

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
};

let reducer: (state, action) => state =
  (state, action) => {
    switch (action) {
    | SetOrderData(orderData) => {...state, orderData}
    | SetEcountData(ecountData) => {...state, ecountData}
    };
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  React.useEffect1(
    () => {
      Js.log(state.orderData);
      None;
    },
    [|state.orderData|],
  );

  React.useEffect1(
    () => {
      Js.log(state.ecountData);
      None;
    },
    [|state.ecountData|],
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
