SELECT
  c.givenName
FROM Families f
JOIN c IN f.children
WHERE 1=1
AND f.id = "WakefieldFamily"
ORDER BY
  f.children.grade ASC
