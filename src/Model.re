type column = array(string);
type row = array(string);
type intColumn = array(int);

type orderData = {
  csvContents: array(column),
  optionManagementCodeColumn: column,
  orderArticleNameColumn: column,
  articleOptionColumn: column,
  orderArticleQtyColumn: column,
  orderArticlePayAmountColumn: column,
};

type ecountData = {
  articleCodeColumn: column,
  eaColumn: intColumn,
  totalColumn: intColumn,
};

type matchResult = {
  optionCodeToArticleCodeMatchResult: array(option(int)),
  orderQtyEaMatchResult: array(bool),
  payAmountTotalMatchResult: array(bool),
};

type state = {
  orderData,
  ecountData,
  matchResult,
  outputContents: option(array(row)),
};

type action =
  | SetOrderData(orderData)
  | SetEcountData(ecountData)
  | SetMatchResult(matchResult)
  | SetOutputContents(option(array(row)));
