UPDATE IGNORE LOW_PRIORITY
  test
JOIN animal ON test.animal_id = animal.id
SET
  foo = ?
, bar = ?
WHERE 1=1
AND moo = "cow"
ORDER BY
  bar DESC
, foo ASC
LIMIT 10
