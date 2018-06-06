SELECT
  {"Name": f.id, "City": f.address.city} AS Family
FROM Families f
WHERE 1=1
AND f.address.city = f.address.state
