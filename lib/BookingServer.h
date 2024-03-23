//
// Created by Maksim Piriyev on 19.03.24.
//

#pragma once
#include "BookingEngine.h"

/**
* @brief RunServer(post) function Runs an http server at the specified port.Default port is 8080.
* It creates a new BookingEngine object and listens to the given port.
* It has several endpoints to get movies, theatres, theatre seats, bookings and to book a seat.
* '/list/movies' endpoint returns the list of movies.
* '/list/theatres' endpoint returns the list of theatres.
* '/list/theatres/:movie_id' endpoint returns the list of theatres for the given movie.
* '/list/theatre-seats/:theatre_id/:movie_id' endpoint returns the list of seats for the given theatre and movie.
* '/list/bookings/:client_id' endpoint returns the list of bookings for the given client.
* '/list/bookings' endpoint returns the list of all bookings.
* '/book' endpoint is used to book seats. It creates a random new client, then books the seats for the movie in theatre. It returns the booking result.
* If the booking is successful, it returns the booking details. If the booking is unsuccessful, it returns the result as false.
* @author Maksim Piriyev
*/
void RunServer(int port = 8080);
