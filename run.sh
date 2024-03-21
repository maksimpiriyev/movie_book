docker build -t movie_book:v1.0 -f Dockerfile .
docker run --name movie_book -d -p 8080:9001 movie_book:v1.0

#example tests
curl --location 'localhost:8080/list/movies'
curl --location 'localhost:9001/list/theatres/1'
curl --location 'localhost:9001/list/bookings'
curl --location 'localhost:9001/book' --header 'Content-Type: application/json' --data '{"theatre_id": 1, "movie_id": 1, "seats" : [3 , 4]}'
curl --location 'localhost:9001/list/bookings'
curl --location 'localhost:9001/book' --header 'Content-Type: application/json' --data '{"theatre_id": 1, "movie_id": 1, "seats" : [3 , 4]}'
curl --location 'localhost:9001/list/bookings'