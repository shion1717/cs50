SELECT name FROM people WHERE id IN
(SELECT person_id
FROM movies JOIN stars ON movies.id = stars.movie_id
WHERE title = "Toy Story");