module Assignment: {
  type t;
  let add: (option(t), string, string) => option(t);
  let render: option(t) => option(string);
};

module Fields: {
  type t;
  let add: (option(t), string) => option(t);
  let render: option(t) => option(string);
};

module From: {
  type t;
  let make: string => t;
  let render: option(t) => option(string);
  let raw: option(t) => option(string);
};

module GroupBy: {
  type t;
  let add: (option(t), string) => option(t);
  let render: option(t) => option(string);
};

module Join: {
  type t;
  let add: (option(t), string) => option(t);
  let render: option(t) => option(string);
};

module Limit: {
  type t;
  let withCount: int => t;
  let withOffset: (int, int) => t;
  let render: option(t) => option(string);
};

module type FlagType = {type flag; let toString: flag => string;};

module Modifier:
  (Flag: FlagType) =>
  {
    type t;
    let add: (option(t), Flag.flag) => option(t);
    let render: option(t) => option(string);
  };

module OrderBy: {
  type t;
  let add: (option(t), [ | `Asc(string) | `Desc(string)]) => option(t);
  let render: option(t) => option(string);
};

module Table: {
  type t;
  let make: string => t;
  let render: option(t) => option(string);
};

module Where: {
  type t;
  let add: (option(t), string) => option(t);
  let render: option(t) => option(string);
};
