[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

type column = array(string);
type csvFileContent = array(column);

[@bs.module "./renderer"]
external openOrderCSVFile: ((csvFileContent, column, column) => unit) => unit =
  "openOrderCSVFile";

type state = {
  csvFileContent,
  optionManagementCodeColumn: column,
  orderArticleQtyColumn: column,
};

type action =
  | SetCSVFileContent(csvFileContent)
  | SetOptionManagementCodeColumn(column)
  | SetOrderArticleQtyColumn(column);

let initialState = {
  csvFileContent: [|[|"", ""|]|],
  optionManagementCodeColumn: [||],
  orderArticleQtyColumn: [||],
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
      None;
    },
    [|state.optionManagementCodeColumn|],
  );

  <div>
    <p>
      {j|NB 검수 시스템에 오신 것을 환영합니다.|j}->React.string
    </p>
    <button
      onClick={_ =>
        openOrderCSVFile(
          (csvFileContent, optionManagementCodeColumn, orderArticleQtyColumn) => {
          dispatch(SetCSVFileContent(csvFileContent));
          dispatch(
            SetOptionManagementCodeColumn(optionManagementCodeColumn),
          );
          dispatch(SetOrderArticleQtyColumn(orderArticleQtyColumn));
        })
      }>
      {j|쇼핑몰 주문 csv 파일 선택|j}->React.string
    </button>
  </div>;
};
