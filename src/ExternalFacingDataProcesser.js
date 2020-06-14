const array = require("lodash/array");
const XLSX = require("xlsx");

export function csvContentsToOrderDataState(csvContents) {
  const headerRow = csvContents[0];
  const optionManagementCodeIndex = array.findIndex(
    headerRow,
    (columnName) => columnName === "옵션 관리코드"
  );
  const orderArticleNameIndex = array.findIndex(
    headerRow,
    (columnName) => columnName === "주문상품명"
  );
  const articleOptionIndex = array.findIndex(
    headerRow,
    (columnName) => columnName === "상품옵션"
  );
  const orderArticleQtyIndex = array.findIndex(
    headerRow,
    (columnName) => columnName === "주문품목 수량"
  );
  const orderArticlePayAmountIndex = array.findIndex(
    headerRow,
    (columnName) => columnName === "주문품목 결제금액"
  );

  const optionManagementCodeColumn = array.tail(
    csvContents.map((record) => record[optionManagementCodeIndex])
  );
  const orderArticleNameColumn = array.tail(
    csvContents.map((record) => record[orderArticleNameIndex])
  );
  const articleOptionColumn = array.tail(
    csvContents.map((record) => record[articleOptionIndex])
  );
  const orderArticleQtyColumn = array.tail(
    csvContents.map((record) => record[orderArticleQtyIndex])
  );
  const orderArticlePayAmountColumn = array.tail(
    csvContents.map((record) => record[orderArticlePayAmountIndex])
  );

  return {
    csvContents,
    optionManagementCodeColumn,
    orderArticleNameColumn,
    articleOptionColumn,
    orderArticleQtyColumn,
    orderArticlePayAmountColumn,
  };
}

export function excelWorkbookToEcountDataState(workbook) {
  const currentSalesReport = XLSX.utils.sheet_to_json(
    workbook.Sheets.판매현황,
    { range: 1 }
  );

  const articleCodeColumn = currentSalesReport.map((record) => record.품목코드);
  const eaColumn = currentSalesReport.map((record) => record.EA);
  const totalColumn = currentSalesReport.map((record) => record.합계);

  return {
    articleCodeColumn,
    eaColumn,
    totalColumn,
  };
}
