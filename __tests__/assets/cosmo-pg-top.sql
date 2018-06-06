SELECT
  TOP 20 food.id
, food.description
, food.tags
, food.foodGroup
FROM food
WHERE 1=1
AND food.foodGroup = "Snacks"
