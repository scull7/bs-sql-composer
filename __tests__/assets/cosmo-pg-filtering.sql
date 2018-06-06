SELECT
  food.id
, food.description
, food.tags
, food.foodGroup
, food.manufacturerName
, food.version
FROM food
WHERE 1=1
AND
          (
            food.manufacturerName = "The Coca-Cola Company"
            AND food.version > 0
          )
