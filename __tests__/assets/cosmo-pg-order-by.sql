SELECT
  food.description
, food.foodGroup
, food.servings[0].description AS servingDescription
, food.servings[0].weightInGrams AS servingWeight
FROM food
WHERE 1=1
AND food.foodGroup = "Fruits and Fruit Juices"
AND food.servings[0].description = "cup"
ORDER BY
  food.servings[0].weightInGrams DESC
