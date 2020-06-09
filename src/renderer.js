/**
 * This file will automatically be loaded by webpack and run in the "renderer" context.
 * To learn more about the differences between the "main" and the "renderer" context in
 * Electron, visit:
 *
 * https://electronjs.org/docs/tutorial/application-architecture#main-and-renderer-processes
 *
 * By default, Node.js integration in this file is disabled. When enabling Node.js integration
 * in a renderer process, please be aware of potential security implications. You can read
 * more about security risks here:
 *
 * https://electronjs.org/docs/tutorial/security
 *
 * To enable Node.js integration in this file, open up `main.js` and enable the `nodeIntegration`
 * flag:
 *
 * ```
 *  // Create the browser window.
 *  mainWindow = new BrowserWindow({
 *    width: 800,
 *    height: 600,
 *    webPreferences: {
 *      nodeIntegration: true
 *    }
 *  });
 * ```
 */

import "./index.css";
import * as React from "./Index.bs";

console.log = function () {};

var app = require("electron").remote;
var dialog = app.dialog;
var fs = require("fs");
const parse = require("csv-parse/lib/sync");
const stringify = require("csv-stringify/lib/sync");
const iconv = require("iconv-lite");
var _ = require("lodash/core");
const array = require("lodash/array");
const XLSX = require("xlsx");

export function openOrderCSVFile(setState) {
  const filepaths = dialog.showOpenDialogSync({
    filters: [{ name: "CSV files", extensions: ["csv"] }],
    properties: ["openFile"],
  });

  if (!filepaths || filepaths.length == 0) {
    console.log("No file selected");
    return;
  }

  const filepath = filepaths[0];

  const originalFile = fs.readFileSync(filepath);
  const decoded = iconv.decode(originalFile, "EUC-KR");

  const csvContents = parse(decoded, {
    bom: true,
  });

  const headerRow = csvContents[0];
  const optionManagementCodeIndex = array.findIndex(
    headerRow,
    (columnName) => columnName === "옵션 관리코드"
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

  const orderArticleQtyColumn = array.tail(
    csvContents.map((record) => record[orderArticleQtyIndex])
  );

  const orderArticlePayAmountColumn = array.tail(
    csvContents.map((record) => record[orderArticlePayAmountIndex])
  );

  setState({
    csvContents,
    optionManagementCodeColumn,
    orderArticleQtyColumn,
    orderArticlePayAmountColumn,
  });
}

export function openEcountExcelFile(setEcountData) {
  const filepaths = dialog.showOpenDialogSync({
    filters: [{ name: "Excel files", extensions: ["xlsx", "xls"] }],
    properties: ["openFile"],
  });

  if (!filepaths || filepaths.length == 0) {
    console.log("No file selected");
    return;
  }
  const filepath = filepaths[0];
  const workbook = XLSX.readFile(filepath);
  const currentSalesReport = XLSX.utils.sheet_to_json(
    workbook.Sheets.판매현황,
    { range: 1 }
  );

  const articleCodeColumn = currentSalesReport.map((record) => record.품목코드);
  const eaColumn = currentSalesReport.map((record) => record.EA);
  const totalColumn = currentSalesReport.map((record) => record.합계);

  setEcountData({
    articleCodeColumn,
    eaColumn,
    totalColumn,
  });
}

export function saveOutputFile(outputContents) {
  const filepath = dialog.showSaveDialogSync({
    filters: [{ name: "CSV files", extensions: ["csv"] }],
    properties: ["createDirectory", "showOverwriteConfirmation"],
  });

  if (filepath) {
    fs.writeFileSync(filepath, stringify(outputContents));
  }
}
