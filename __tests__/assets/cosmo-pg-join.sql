SELECT
  tag.name
FROM food
JOIN tag IN food.tags
WHERE 1=1
AND food.id = "09052"
