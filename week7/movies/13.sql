SELECT DISTINCT name FROM people
INNER JOIN stars
ON people.id = stars.person_id
INNER JOIN movies
ON movies.id = stars.movie_id
WHERE movie_id in (SELECT movie_id FROM people
INNER JOIN stars
ON people.id = stars.person_id
INNER JOIN movies
ON movies.id = stars.movie_id
WHERE movie_id in (SELECT movie_id WHERE person_id in (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958)))
AND name NOT LIKE "Kevin Bacon";