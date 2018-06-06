SELECT
  food.id
, food.description
, food.tags
, food.foodGroup
, food.version
FROM food
WHERE 1=1
AND food.foodGroup IN ("Poultry Products", "Sausages and Luncheon Meats")
AND (food.id BETWEEN "05740" AND "07050")
