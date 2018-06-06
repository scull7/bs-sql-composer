open Jest;

let genFileTest = SqlComposerTest.genFileTest;

let url = "https://docs.microsoft.com/en-us/azure/cosmos-db/sql-api-sql-query";

describe("CosmoDB", () => {
  describe("Tutorial Example Queries", () => {
    genFileTest("Example - 1", "cosmo-example-1", () =>
      SqlComposer.Select.(
        make()
        |. field("*")
        |. from("Families f")
        |. where({|AND f.id = "WakefieldFamily"|})
        |. toSql
      )
    );

    genFileTest("Example - 2", "cosmo-example-2", () =>
      SqlComposer.Select.(
        make()
        |. field("c.givenName")
        |. from("Families f")
        |. join("JOIN c IN f.children")
        |. where({|AND f.id = "WakefieldFamily"|})
        |. orderBy(`Asc("f.children.grade"))
        |. toSql
      )
    );
  });

  describe({j|SQL queries for Azure Cosmos DB: $url|j}, () => {
    genFileTest("Getting Started - 1", "cosmo-gs-1", () =>
      SqlComposer.Select.(
        make()
        |. field("*")
        |. from("Families f")
        |. where({|AND f.id = "AndersenFamily"|})
        |. toSql
      )
    );

    genFileTest("Getting Started - 2", "cosmo-gs-2", () =>
      SqlComposer.Select.(
        make()
        |. field({|{"Name": f.id, "City": f.address.city} AS Family|})
        |. from("Families f")
        |. where({|AND f.address.city = f.address.state|})
        |. toSql
      )
    );
  });

  describe("documentdb.com Query Playground", () => {
    genFileTest("Syntax", "cosmo-pg-1", () =>
      SqlComposer.Select.(
        make()
        |. field("food.id")
        |. field("food.description")
        |. field("food.tags")
        |. field("food.foodGroup")
        |. from("food")
        |. where({|AND food.foodGroup = "Snacks"|})
        |. where({|AND food.id = "19015"|})
        |. toSql
      )
    );

    genFileTest("Filtering", "cosmo-pg-filtering", () =>
      SqlComposer.Select.(
        make()
        |. field("food.id")
        |. field("food.description")
        |. field("food.tags")
        |. field("food.foodGroup")
        |. field("food.manufacturerName")
        |. field("food.version")
        |. from("food")
        |. where(
             {|AND
          (
            food.manufacturerName = "The Coca-Cola Company"
            AND food.version > 0
          )|},
           )
        |. toSql
      )
    );

    genFileTest("Order By", "cosmo-pg-order-by", () =>
      SqlComposer.Select.(
        make()
        |. field("food.description")
        |. field("food.foodGroup")
        |. field("food.servings[0].description AS servingDescription")
        |. field("food.servings[0].weightInGrams AS servingWeight")
        |. from("food")
        |. where({|AND food.foodGroup = "Fruits and Fruit Juices"|})
        |. where({|AND food.servings[0].description = "cup"|})
        |. orderBy(`Desc("food.servings[0].weightInGrams"))
        |. toSql
      )
    );

    genFileTest("Top", "cosmo-pg-top", () =>
      SqlComposer.Select.(
        make()
        |. field("TOP 20 food.id")
        |. field("food.description")
        |. field("food.tags")
        |. field("food.foodGroup")
        |. from("food")
        |. where({|AND food.foodGroup = "Snacks"|})
        |. toSql
      )
    );

    genFileTest("In & Between", "cosmo-pg-in-between", () =>
      SqlComposer.Select.(
        make()
        |. field("food.id")
        |. field("food.description")
        |. field("food.tags")
        |. field("food.foodGroup")
        |. field("food.version")
        |. from("food")
        |. where(
             {|AND food.foodGroup IN ("Poultry Products", "Sausages and Luncheon Meats")|},
           )
        |. where({|AND (food.id BETWEEN "05740" AND "07050")|})
        |. toSql
      )
    );

    genFileTest("Projection", "cosmo-pg-projection", () =>
      SqlComposer.Select.(
        make()
        |. field(
             {|{
           "Company": food.manufacturerName,
           "Brand": food.commonName,
           "Serving Description": food.servings[0].description,
           "Serving in Grams": food.servings[0].weightInGrams,
           "Food Group": food.foodGroup
         } AS Food|},
           )
        |. from("food")
        |. where({|AND food.id = "21421"|})
        |. toSql
      )
    );

    genFileTest("Join", "cosmo-pg-join", () =>
      SqlComposer.Select.(
        make()
        |. field("tag.name")
        |. from("food")
        |. join("JOIN tag IN food.tags")
        |. where({|AND food.id = "09052"|})
        |. toSql
      )
    );

    genFileTest("Built-in", "cosmo-pg-built-in", () =>
      SqlComposer.Select.(
        make()
        |. field("food.id")
        |. field("food.commonName")
        |. field("food.foodGroup")
        |. field("ROUND(nutrient.nutritionValue) AS amount")
        |. field("nutrient.units")
        |. from("food")
        |. where({|AND IS_DEFINED(food.commonName)|})
        |. where({|AND nutrient.description = "Water"|})
        |. where(
             {|AND food.foodGroup IN ("Sausages and Luncheon Meats", "Legumes and Legume Products")|},
           )
        |. where({|AND food.id > "42178"|})
        |. join("JOIN nutrient IN food.nutrients")
        |. toSql
      )
    );

    genFileTest("UDF", "cosmo-pg-udf", () =>
      SqlComposer.Select.(
        make()
        |. field("food.id")
        |. field("food.description")
        |. field("food.tags")
        |. field("food.foodGroup")
        |. from("food")
        |. where({|AND food.foodGroup = "Baked Products"|})
        |. where(
             {|AND udf.MATCH(food.description, "Bagels, cinnamon-*") != null|},
           )
        |. toSql
      )
    );

    genFileTest("Geospatial", "cosmo-pg-geospatial", () =>
      SqlComposer.Select.(
        make()
        |. field("*")
        |. from("volcanoes v")
        |. where(
             {|AND
          ST_DISTANCE(v.Location, {
            "type": "Point",
            "coordinates": [-122.19, 47.36]
          }) < 100 * 1000|},
           )
        |. where({|AND v.Type = "Stratovolcano"|})
        |. where(
             {|AND v["Last Known Eruption"] = "Last known eruption from 1800-1899, inclusive"|},
           )
        |. toSql
      )
    );
  });
});
