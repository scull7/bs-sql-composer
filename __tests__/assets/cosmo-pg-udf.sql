SELECT
  food.id
, food.description
, food.tags
, food.foodGroup
FROM food
WHERE 1=1
AND food.foodGroup = "Baked Products"
AND udf.MATCH(food.description, "Bagels, cinnamon-*") != null
