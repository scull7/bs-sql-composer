[![Build Status](https://www.travis-ci.org/scull7/bs-sql-composer.svg?branch=master)](https://www.travis-ci.org/scull7/bs-sql-composer)
[![Coverage Status](https://coveralls.io/repos/github/scull7/bs-sql-composer/badge.svg?branch=master)](https://coveralls.io/github/scull7/bs-sql-composer?branch=master)

# bs-sql-composer
A library for SQL composition in BucklesScript

## Usage

### Basic Select
```reason
  SqlComposer.Select.(
    make()
    |. field("*")
    |. from("test")
    |. toSql
    |. Js.log
  );
```
```sql
SELECT
  *
FROM test
WHERE 1=1
```

### Where Clause
There is an implicit `WHERE 1=1` added to every query.  This is done to
eliminate confusion with compound queries regarding the concatenation of
where clauses.  
```reason
  SqlComposer.Select.(
    make()
    |. field("*")
    |. from("test")
    |. where("AND test.foo = ?")
    |. toSql
    |. Js.log
  );
```
```sql
SELECT
  *
FROM test
WHERE 1=1
AND test.foo = ?
```

### Join Clause
```reasons
  SqlComposer.Select.(
    make()
    |. field("*")
    |. from("test")
    |. join("JOIN foo ON test.foo_id = foo.id")
    |. toSql
    |. Js.log
  )
```
```sql
SELECT
  *
FROM test
JOIN foo ON test.foo_id = foo.id
WHERE 1=1
```

### Adding to a base query
```reason
  let base_query = SqlComposer.Select.(
    make()
    |. from("test")
    |. field("foo")
    |. field("bar")
  );

  SqlComposer.Select.(
    base_query
    |. where("AND test.foo = ?")
    |. toSql
    |. Js.log
  );
```
```sql
SELECT
  foo
, bar
FROM test
WHERE 1=1
AND test.foo = ?
```

### Alias a field
```reason
SqlComposer.Select.(
  make()
  |. from("test")
  |. field("foo AS bar")
  |. toSql
  |. Js.log
);
```
```sql
SELECT
  foo AS bar
FROM test
WHERE 1=1
```

### Order By
```ocaml
SqlComposer.Select.(
  make()
  |. from("test")
  |. field("*")
  |. orderBy(`Asc("foo"))
  |. orderBy(`Desc("bar"))
  |. toSql
  |. Js.log
);
```
```sql
SELECT
  *
FROM test
WHERE 1=1
ORDER BY
  foo ASC
, bar DESC
```

### Group By
```reason
SqlComposer.Select.(
  make()
  |. from("test")
  |. field("foo AS bar")
  |. groupBy("foo")
  |. groupBy("thing")
);
```
```sql
SELECT
  foo AS bar
FROM test
WHERE 1=1
GROUP BY
  foo
, thing
```

### Select Distinct
```reason
SqlComposer.Select.(
  make()
  |. field("foo")
  |. from("test")
  |. modifier(`Distinct)
);
```
```sql
SELECT DISTINCT
  foo
FROM test
WHERE 1=1
```

## How do I install it?

Inside of a BuckleScript project:

```shell
yarn install --save bs-sql-composer
```

Then add `bs-sql-composer` to your `bs-dependencies` in `bsconfig.json`:

```json
{
  "bs-dependencies": [ "bs-sql-composer" ]
}
```
