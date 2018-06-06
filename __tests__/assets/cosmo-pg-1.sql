SELECT
  food.id
, food.description
, food.tags
, food.foodGroup
FROM food
WHERE 1=1
AND food.foodGroup = "Snacks"
AND food.id = "19015"
