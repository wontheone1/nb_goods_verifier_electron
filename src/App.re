[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

type column = array(string);
type csvFileContent = array(column);

[@bs.module "./renderer"]
external openOrderCSVFile: ((csvFileContent, int, column) => unit) => unit =
  "openOrderCSVFile";

type state = {
  csvFileContent,
  optionManagementCodeIndex: int,
  optionManagementCodeColumn: column,
};

type action =
  | SetCSVFileContent(csvFileContent)
  | SetOptionManagementCodeIndex(int)
  | SetOptionManagementCodeColumn(array(string));

let initialState = {
  csvFileContent: [|[|"", ""|]|],
  optionManagementCodeIndex: (-1),
  optionManagementCodeColumn: [||],
};

let reducer: (state, action) => state =
  (state, action) => {
    switch (action) {
    | SetCSVFileContent(csvFileContent) => {...state, csvFileContent}
    | SetOptionManagementCodeIndex(optionManagementCodeIndex) => {
        ...state,
        optionManagementCodeIndex,
      }
    | SetOptionManagementCodeColumn(optionManagementCodeColumn) => {
        ...state,
        optionManagementCodeColumn,
      }
    };
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  React.useEffect1(
    () => {
      Js.log(state.csvFileContent);
      Js.log(state.optionManagementCodeIndex);
      Js.log(state.optionManagementCodeColumn);
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
          (
            csvFileContent,
            optionManagementCodeIndex,
            optionManagementCodeColumn,
          ) => {
          dispatch(SetCSVFileContent(csvFileContent));
          dispatch(SetOptionManagementCodeIndex(optionManagementCodeIndex));
          dispatch(
            SetOptionManagementCodeColumn(optionManagementCodeColumn),
          );
        })
      }>
      {j|쇼핑몰 주문 csv 파일 선택|j}->React.string
    </button>
  </div>;
};
