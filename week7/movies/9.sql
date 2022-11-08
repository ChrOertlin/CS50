SELECT DISTINCT name FROM people
INNER JOIN stars
on people.id = stars.person_id
INNER JOIN movies
on movies.id = stars.movie_id
WHERE year == 2004
ORDER BY birth;