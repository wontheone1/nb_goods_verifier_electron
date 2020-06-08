[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

type column = array(string);
type csvFileContent = array(column);

type ecountData = {
  articleCodeColumn: column,
  eaColumn: column,
  totalColumn: column,
};

type state = {
  csvFileContent,
  optionManagementCodeColumn: column,
  orderArticleQtyColumn: column,
  orderArticlePayAmountIndexColumn: column,
  ecountData,
};

[@bs.module "./renderer"]
external openOrderCSVFile:
  ((csvFileContent, column, column, column) => unit) => unit =
  "openOrderCSVFile";

[@bs.module "./renderer"]
external openEcountExcelFile: (ecountData => unit) => unit =
  "openEcountExcelFile";

type action =
  | SetCSVFileContent(csvFileContent)
  | SetOptionManagementCodeColumn(column)
  | SetOrderArticleQtyColumn(column)
  | SetOrderArticlePayAmountIndexColumn(column)
  | SetEcountData(ecountData);

let initialState = {
  csvFileContent: [|[|"", ""|]|],
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
    | SetCSVFileContent(csvFileContent) => {...state, csvFileContent}
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
      Js.log(state.csvFileContent);
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
            csvFileContent,
            optionManagementCodeColumn,
            orderArticleQtyColumn,
            orderArticlePayAmountIndexColumn,
          ) => {
          dispatch(SetCSVFileContent(csvFileContent));
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
