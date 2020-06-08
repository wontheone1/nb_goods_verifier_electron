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

  setState(records);
  console.log(records);
}

console.log(
  '👋 This message is being logged by "renderer.js", included via webpack'
);
