SELECT continent, country, city, SUM(units_sold) AS total_sold
FROM prod_sales
GROUP BY CUBE(continent, country, city)
ORDER BY city;