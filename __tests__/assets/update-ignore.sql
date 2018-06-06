UPDATE IGNORE
  test
SET
  foo = ?
, bar = ?
WHERE 1=1
AND moo = "cow"
ORDER BY
  bar DESC
, foo ASC
