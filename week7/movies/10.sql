SELECT DISTINCT name FROM people
INNER JOIN directors
on people.id = directors.person_id
INNER JOIN ratings
on ratings.movie_id = directors.movie_id
WHERE rating >= 9.0;