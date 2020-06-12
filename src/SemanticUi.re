module Button = {
  [@bs.module "semantic-ui-react"] [@react.component]
  external make:
    (~children: React.element, ~onClick: ReactEvent.Mouse.t => unit=?) =>
    React.element =
    "Button";
};

module Header = {
  [@bs.module "semantic-ui-react"] [@react.component]
  external make: (~children: React.element, ~_as: string=?) => React.element =
    "Header";
};

module Icon = {
  [@bs.module "semantic-ui-react"] [@react.component]
  external make: (~children: React.element=?, ~name: string=?) => React.element =
    "Icon";
};

module Input = {
  type inputLabel = {
    basic: bool,
    content: string,
  };
  type inputAction = {
    content: string,
    onClick: ReactEvent.Mouse.t => unit,
  };
  [@bs.module "semantic-ui-react"] [@react.component]
  external make:
    (
      ~action: inputAction=?,
      ~children: React.element=?,
      ~icon: string=?,
      ~id: string=?,
      ~label: inputLabel=?,
      ~onChange: ReactEvent.Form.t => unit=?,
      ~placeholder: string=?,
      ~value: string=?
    ) =>
    React.element =
    "Input";
};

module Menu = {
  [@bs.module "semantic-ui-react"] [@react.component]
  external make:
    (~children: React.element, ~secondary: bool=?) => React.element =
    "Menu";

  module Item = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Menu"] [@react.component]
    external make:
      (
        ~active: bool=?,
        ~children: React.element=?,
        ~name: string=?,
        ~onClick: ReactEvent.Mouse.t => unit=?
      ) =>
      React.element =
      "Item";
  };

  module Menu = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Menu"] [@react.component]
    external make:
      (~children: React.element, ~position: string) => React.element =
      "Menu";
  };
};

module Modal = {
  [@bs.module "semantic-ui-react"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~basic: bool=?,
      ~size: string=?,
      ~trigger: React.element=?
    ) =>
    React.element =
    "Modal";

  module Content = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Modal"] [@react.component]
    external make: (~children: React.element) => React.element = "Content";
  };
};

module Tab = {
  module Pane = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Tab"] [@react.component]
    external make: (~attached: bool, ~children: React.element) => React.element =
      "Pane";
  };

  type menu = {
    secondary: bool,
    pointing: bool,
  };

  type pane = {
    menuItem: string,
    render: unit => React.element,
  };

  [@bs.module "semantic-ui-react"] [@react.component]
  external make: (~menu: menu, ~panes: array(pane)) => React.element = "Tab";
};

module Table = {
  [@bs.module "semantic-ui-react"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~sortable: bool,
      ~celled: bool,
      ~fixed: bool,
      ~onClick: ReactEvent.Mouse.t => unit=?
    ) =>
    React.element =
    "Table";

  module Header = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Table"] [@react.component]
    external make:
      (
        ~children: React.element,
        ~sortable: bool=?,
        ~celled: bool=?,
        ~fixed: bool=?,
        ~onClick: ReactEvent.Mouse.t => unit=?
      ) =>
      React.element =
      "Header";
  };

  module Row = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Table"] [@react.component]
    external make:
      (
        ~children: React.element,
        ~sortable: bool=?,
        ~celled: bool=?,
        ~fixed: bool=?,
        ~onClick: ReactEvent.Mouse.t => unit=?
      ) =>
      React.element =
      "Row";
  };

  type sortDirection =
    | Ascending
    | Descending
    | NoDirection;

  module HeaderCell = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Table"] [@react.component]
    external make:
      (
        ~children: React.element,
        ~sorted: sortDirection=?,
        ~celled: bool=?,
        ~fixed: bool=?,
        ~onClick: ReactEvent.Mouse.t => unit=?
      ) =>
      React.element =
      "HeaderCell";
  };

  module Body = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Table"] [@react.component]
    external make:
      (
        ~children: React.element,
        ~sorted: Js.Nullable.t(string)=?,
        ~celled: bool=?,
        ~fixed: bool=?,
        ~onClick: ReactEvent.Mouse.t => unit=?
      ) =>
      React.element =
      "Body";
  };

  module Cell = {
    [@bs.module "semantic-ui-react"] [@bs.scope "Table"] [@react.component]
    external make:
      (
        ~children: React.element,
        ~sorted: Js.Nullable.t(string)=?,
        ~celled: bool=?,
        ~fixed: bool=?,
        ~onClick: ReactEvent.Mouse.t => unit=?
      ) =>
      React.element =
      "Cell";
  };
};
