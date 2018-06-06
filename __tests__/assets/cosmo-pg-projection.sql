SELECT
  {
           "Company": food.manufacturerName,
           "Brand": food.commonName,
           "Serving Description": food.servings[0].description,
           "Serving in Grams": food.servings[0].weightInGrams,
           "Food Group": food.foodGroup
         } AS Food
FROM food
WHERE 1=1
AND food.id = "21421"
