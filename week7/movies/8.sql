SELECT name FROM people
INNER JOIN stars
on people.id = stars.person_id
INNER JOIN movies
on movies.id = stars.movie_id
WHERE title == "Toy Story";