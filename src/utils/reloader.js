import { app, ipcMain } from "electron";
import path from "path";

import { mainReloader, rendererReloader } from "electron-hot-reload";

const mainFile = path.join(app.getAppPath(), ".webpack", "main", "index.js");
const rendererFile = path.join(app.getAppPath(), ".webpack", "renderer", "main_window", "index.js");

mainReloader(mainFile, undefined, (error, path) => {
  console.log("It is a main's process hook!");
});

rendererReloader(rendererFile, undefined, (error, path) => {
  console.log("It is a renderer's process hook!");
});
