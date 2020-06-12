// Entry point

[@bs.val] external document: Js.t({..}) = "document";
// import 'semantic-ui-css/semantic.min.css'
[@bs.module "semantic-ui-css/semantic.min.css"]
external semantic: Js.t({..}) = "default";

let semantic = semantic;

ReactDOMRe.renderToElementWithId(<App />, "app");
