SELECT title FROM movies
INNER JOIN stars
on movies.id = stars.movie_id
INNER JOIN people
on people.id = stars.person_id
INNER JOIN ratings
on ratings.movie_id = stars.movie_id
WHERE name == "Chadwick Boseman"
ORDER BY rating DESC
LIMIT 5;