type state = string;
type action =
  | SetState(string);

let initialState = "hello";

let reducer: (state, action) => state =
  (state, action) => {
    switch (action) {
    | SetState(newState) => newState
    };
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  <div>
    <p>
      {j|NB 검수 시스템에 오신 것을 환영합니다.|j}->React.string
    </p>
  </div>;
};
