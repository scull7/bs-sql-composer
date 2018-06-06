UPDATE
  test
JOIN animal ON test.animal_id = animal.id
SET
  foo = ?
WHERE 1=1
AND cow = "moo"
AND dog = "bark"
ORDER BY
  cat ASC
LIMIT 10 OFFSET 5
