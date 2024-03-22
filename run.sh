echo "Building Dockerfile : movie_book"
docker build -t movie_book:v1.0 -f Dockerfile .

echo "Running docker: movie_book"
docker run --name movie_book -d -p 8080:8080 movie_book:v1.0

#example tests
echo "Example tests"

echo "List of movies"
curl --location 'localhost:8080/list/movies'

echo "List of theatres that shows movie: 1"
curl --location 'localhost:9001/list/theatres/1'

echo "List of empty seats that show movie:1 in theatre:1"
curl --location 'localhost:9001/list/theatre-seats/1/1'

echo "List of bookings@0"
curl --location 'localhost:9001/list/bookings'

echo "Book seats 3 and 4 for movie 1 in theatre 1"
curl --location 'localhost:9001/book' --header 'Content-Type: application/json' --data '{"theatre_id": 1, "movie_id": 1, "seats" : [3 , 4]}'

echo "List of bookings@1"
curl --location 'localhost:9001/list/bookings'

echo "Try to book seats 3 and 4 for movie 1 in theatre 1 one more time "
curl --location 'localhost:9001/book' --header 'Content-Type: application/json' --data '{"theatre_id": 1, "movie_id": 1, "seats" : [3 , 4]}'

echo "List of bookings@2"
curl --location 'localhost:9001/list/bookings'
