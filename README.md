# movie_book

run the command `./run.sh` from command line to docker web server at http://localhost:8080.
This will build a docker image and run a web server on port 8080.
Then it will also send a curl request to the web server to get the list of movies and books.
Additionally, it will also send a curl request to book the movie in a theatre.

It has several endpoints to get movies, theatres, theatre seats, bookings and to book a seat.
'/list/movies' endpoint returns the list of movies.
'/list/theatres' endpoint returns the list of theatres.
'/list/theatres/:movie_id' endpoint returns the list of theatres for the given movie.
'/list/theatre-seats/:theatre_id/:movie_id' endpoint returns the list of seats for the given theatre and movie.
'/list/bookings/:client_id' endpoint returns the list of bookings for the given client.
'/list/bookings' endpoint returns the list of all bookings.
'/book' endpoint is used to book seats. It creates a random new client, then books the seats for the movie in theatre. It returns the booking result. If the booking is successful, it returns the booking details. If the booking is unsuccessful, it returns the result as false.
