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

var app = require("electron").remote;
var dialog = app.dialog;
var fs = require("fs");
const parse = require("csv-parse/lib/sync");
const iconv = require("iconv-lite");
var _ = require("lodash/core");
const array = require("lodash/array");
const XLSX = require("xlsx");

export function openOrderCSVFile(setState) {
  const filepaths = dialog.showOpenDialogSync({
    properties: ["openFile"],
  });

  if (!filepaths || filepaths.length == 0) {
    console.log("No file selected");
    return;
  }

  const filepath = filepaths[0];

  const originalFile = fs.readFileSync(filepath);
  const decoded = iconv.decode(originalFile, "EUC-KR");

  const records = parse(decoded, {
    bom: true,
  });

  const headerRow = records[0];
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

  setState(
    array.take(records, 10),
    records.map((record) => record[optionManagementCodeIndex]),
    records.map((record) => record[orderArticleQtyIndex]),
    records.map((record) => record[orderArticlePayAmountIndex])
  );
}

export function openEcountExcelFile(setState) {
  const filepaths = dialog.showOpenDialogSync({
    properties: ["openFile"],
  });

  if (!filepaths || filepaths.length == 0) {
    console.log("No file selected");
    return;
  }
  const filepath = filepaths[0];
  const workbook = XLSX.readFile(filepath);
  console.log(workbook);
  const currentSalesReport = XLSX.utils.sheet_to_json(
    workbook.Sheets.판매현황,
    { range: 1 }
  );
  console.log(currentSalesReport);
}

console.log(
  '👋 This message is being logged by "renderer.js", included via webpack'
);
