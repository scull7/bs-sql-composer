SELECT
  food.id
, food.commonName
, food.foodGroup
, ROUND(nutrient.nutritionValue) AS amount
, nutrient.units
FROM food
JOIN nutrient IN food.nutrients
WHERE 1=1
AND IS_DEFINED(food.commonName)
AND nutrient.description = "Water"
AND food.foodGroup IN ("Sausages and Luncheon Meats", "Legumes and Legume Products")
AND food.id > "42178"
