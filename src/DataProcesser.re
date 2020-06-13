let optionToBool: option(int) => bool =
  v =>
    switch (v) {
    | Some(_) => true
    | None => false
    };

let boolToPassFail: bool => string = v => if (v) {"PASS"} else {"FAIL"};

let makeOutputContents = (state: Model.state) => {
  Belt.Array.mapWithIndex(state.orderData.csvContents, (i, row) =>
    if (i == 0) {
      Belt.Array.concat(
        row,
        [|
          {j|검수결과|j},
          {j|품목체크|j},
          {j|수량체크|j},
          {j|단가체크|j},
        |],
      );
    } else {
      let idx = i - 1;
      let articleCodeMatchResult =
        state.matchResult.optionCodeToArticleCodeMatchResult[idx]
        ->optionToBool;
      let qtyCheckResult = state.matchResult.orderQtyEaMatchResult[idx];
      let totalCheckResult = state.matchResult.payAmountTotalMatchResult[idx];
      Belt.Array.concat(
        row,
        [|
          boolToPassFail(
            qtyCheckResult && totalCheckResult && articleCodeMatchResult,
          ),
          boolToPassFail(articleCodeMatchResult),
          boolToPassFail(qtyCheckResult),
          boolToPassFail(totalCheckResult),
        |],
      );
    }
  );
};
