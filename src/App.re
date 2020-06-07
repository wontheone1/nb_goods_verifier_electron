[@bs.module "electron"] [@bs.scope "remote"]
external dialog: unit => unit = "dialog";

[@bs.module "./renderer"]
external openOrderCSVFile: (string => unit) => unit = "openOrderCSVFile";

type state = {csvFileContent: string};
type action =
  | SetState(string);

let initialState = {csvFileContent: ""};

let reducer: (state, action) => state =
  (state, action) => {
    switch (action) {
    | SetState(csvFileContent) => {csvFileContent: csvFileContent}
    };
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  <div>
    <p>
      {j|NB 검수 시스템에 오신 것을 환영합니다.|j}->React.string
    </p>
    <button
      onClick={_ =>
        openOrderCSVFile(csvFileContent =>
          dispatch(SetState(csvFileContent))
        )
      }>
      {j|주문 csv 파일 선택|j}->React.string
    </button>
    <p> state.csvFileContent->React.string </p>
  </div>;
};
