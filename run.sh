docker build -t movie_book:v1.0 -f Dockerfile .
docker run --name movie_book -d -p 8080:9001 movie_book:v1.0