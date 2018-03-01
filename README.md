[![Build Status](https://www.travis-ci.org/scull7/bs-sql-composer.svg?branch=master)](https://www.travis-ci.org/scull7/bs-sql-composer)
[![Coverage Status](https://coveralls.io/repos/github/scull7/bs-sql-composer/badge.svg?branch=master)](https://coveralls.io/github/scull7/bs-sql-composer?branch=master)

# bs-sql-composer
A library for SQL composition in BucklesScript

## Usage

### Basic Select
```ocaml
  let _ = SqlComposer.Query.(
    from "test"
    |> Pipe.select "*"
    |> to_sql
    |> Js.log
  )
```
> "SELECT * FROM test  WHERE 1=1"

### Where Clause
```ocaml
  let _ = SqlComposer.Query.(
    from "test"
    |> Pipe.select "*"
    |> Pipe.where "AND test.foo = ?"
    |> to_sql
    |> Js.log
  )
```
> "SELECT * FROM test WHERE 1=1 AND test.foo = ?"

### Join Clause
```ocaml
  let _ = SqlComposer.Query.(
    from "test"
    |> Pipe.select "*"
    |> Pipe.join "JOIN foo ON test.foo_id = foo.id"
    |> to_sql
    |> Js.log
  )
```
> "SELECT * FROM test JOIN foo ON test.foo_id = foo.id WHERE 1=1"

### Adding to a base query
```ocaml

  let _ = SqlComposer.Query.(
    let base_query =
      from "test"
      |> Pipe.select "foo"
      |> Pipe.select "bar"
    in
    where base_query "AND test.foo = ?"
    |> to_sql
    |> Js.log
  )
```
> "SELECT foo, bar FROM test WHERE 1=1 AND test.foo = ?"
