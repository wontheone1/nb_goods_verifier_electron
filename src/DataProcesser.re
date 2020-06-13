let optionToBool: option(int) => bool =
  v =>
    switch (v) {
    | Some(_) => true
    | None => false
    };

let boolToPassFail: bool => string = v => if (v) {"PASS"} else {"FAIL"};

let makeOutputContents = (state: Model.state) => {
  Belt.Array.concat(
    [|
      [|
        {j|주문상품명|j},
        {j|상품옵션|j},
        {j|검수결과|j},
        {j|품목체크|j},
        {j|수량체크|j},
        {j|단가체크|j},
      |],
    |],
    Belt.Array.mapWithIndex(
      state.orderData.orderArticleNameColumn,
      (i, row) => {
        let articleCodeMatchResult =
          state.matchResult.optionCodeToArticleCodeMatchResult[i]
          ->optionToBool;
        let qtyCheckResult = state.matchResult.orderQtyEaMatchResult[i];
        let totalCheckResult = state.matchResult.payAmountTotalMatchResult[i];

        [|
          row,
          state.orderData.articleOptionColumn[i],
          boolToPassFail(
            articleCodeMatchResult && qtyCheckResult && totalCheckResult,
          ),
          boolToPassFail(articleCodeMatchResult),
          boolToPassFail(qtyCheckResult),
          boolToPassFail(totalCheckResult),
        |];
      },
    ),
  );
};

let resultToPassFail =
  fun
  | Model.Fail => "FAIL"
  | Model.Pass => "PASS";

let filterOutputContentsByCheckResult =
    (outputContents: array(array(string)), filter: Model.result) => {
  let filter = resultToPassFail(filter);
  Belt.Array.keepWithIndex(outputContents, (row, i) => {
    row[2] == filter || i == 0
  });
};
