[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

type column = array(string);

type ecountData = {
  articleCodeColumn: column,
  eaColumn: column,
  totalColumn: column,
};

type state = {
  optionManagementCodeColumn: column,
  orderArticleQtyColumn: column,
  orderArticlePayAmountIndexColumn: column,
  ecountData,
};

[@bs.module "./renderer"]
external openOrderCSVFile: ((column, column, column) => unit) => unit =
  "openOrderCSVFile";

[@bs.module "./renderer"]
external openEcountExcelFile: (ecountData => unit) => unit =
  "openEcountExcelFile";

type action =
  | SetOptionManagementCodeColumn(column)
  | SetOrderArticleQtyColumn(column)
  | SetOrderArticlePayAmountIndexColumn(column)
  | SetEcountData(ecountData);

let initialState = {
  optionManagementCodeColumn: [||],
  orderArticleQtyColumn: [||],
  orderArticlePayAmountIndexColumn: [||],
  ecountData: {
    articleCodeColumn: [||],
    eaColumn: [||],
    totalColumn: [||],
  },
};

let reducer: (state, action) => state =
  (state, action) => {
    switch (action) {
    | SetOptionManagementCodeColumn(optionManagementCodeColumn) => {
        ...state,
        optionManagementCodeColumn,
      }
    | SetOrderArticleQtyColumn(orderArticleQtyColumn) => {
        ...state,
        orderArticleQtyColumn,
      }
    | SetOrderArticlePayAmountIndexColumn(orderArticlePayAmountIndexColumn) => {
        ...state,
        orderArticlePayAmountIndexColumn,
      }
    | SetEcountData(ecountData) => {...state, ecountData}
    };
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  React.useEffect1(
    () => {
      Js.log(state.optionManagementCodeColumn);
      Js.log(state.orderArticleQtyColumn);
      Js.log(state.orderArticlePayAmountIndexColumn);
      None;
    },
    [|state.optionManagementCodeColumn|],
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
        openOrderCSVFile(
          (
            optionManagementCodeColumn,
            orderArticleQtyColumn,
            orderArticlePayAmountIndexColumn,
          ) => {
          dispatch(
            SetOptionManagementCodeColumn(optionManagementCodeColumn),
          );
          dispatch(SetOrderArticleQtyColumn(orderArticleQtyColumn));
          dispatch(
            SetOrderArticlePayAmountIndexColumn(
              orderArticlePayAmountIndexColumn,
            ),
          );
        })
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
