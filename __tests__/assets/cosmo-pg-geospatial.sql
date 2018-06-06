SELECT
  *
FROM volcanoes v
WHERE 1=1
AND
          ST_DISTANCE(v.Location, {
            "type": "Point",
            "coordinates": [-122.19, 47.36]
          }) < 100 * 1000
AND v.Type = "Stratovolcano"
AND v["Last Known Eruption"] = "Last known eruption from 1800-1899, inclusive"
