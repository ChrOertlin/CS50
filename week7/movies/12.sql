SELECT title FROM movies
INNER JOIN stars
on movies.id = stars.movie_id
INNER JOIN people
on people.id = stars.person_id
WHERE name in ("Johnny Depp","Helena Bonham Carter")
GROUP BY title
HAVING COUNT(DISTINCT NAME) = 2;