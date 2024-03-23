//
// Created by Maksim Piriyev on 18.03.24.
//

#pragma once

#include <sstream>
#include <vector>
#include <map>
#include <semaphore>
#include <memory>
#include <format>
#include "BookingLibrary.h"

using namespace std;

struct TheatreSeats {
    int movie_id;
    int theatre_id;
    vector<int> empty_seats;
};

class BookingEngine {
    vector<shared_ptr<Booking>> bookings;

    map<int, shared_ptr<Movie>> mapMovies;
    map<int, shared_ptr<Theatre>> mapTheatres;
    map<int, shared_ptr<Client>> mapClients;
    map<int, vector<shared_ptr<Theatre>>> mapMovieTheatres;
public:


    /**
    * new_client method creates a new client with a random name and returns it
    * @author Maksim Piriyev
    */
    shared_ptr<Client> new_client() {
        auto id = mapClients.size() + 1;
        stringstream stream;
        stream << "Random Client " << id;
        mapClients[id] = make_shared<Client>(id, stream.str());
        return mapClients[id];
    }

    /**
    * get_movies method returns all movies in the system
    * @author Maksim Piriyev
    */
    const vector<shared_ptr<Movie>> get_movies() {
        vector<shared_ptr<Movie>> list;
        for (auto [_, movie]: mapMovies) {
            list.push_back(movie);
        }
        return list;
    }

    /**
    * get_theatres method returns all theatres in the system
    * @author Maksim Piriyev
    */
    const vector<shared_ptr<Theatre>> get_theatres() {
        vector<shared_ptr<Theatre>> list;
        for (auto [_, theatre]: mapTheatres) {
            list.push_back(theatre);
        }
        return list;
    }

    /**
    * get_theatre method returns theatre with theatre_id
    * @author Maksim Piriyev
    */
    const shared_ptr<Theatre> get_theatre(int theatre_id) {
        return mapTheatres[theatre_id];
    }

    /**
    * get_theatres(movie_id) method returns all theatres in the system that has the movie with the given id
    * @params movie_id
    * @author Maksim Piriyev
    */
    const vector<shared_ptr<Theatre>> get_theatres(int movie_id) {
        return this->mapMovieTheatres[movie_id];
    }

    /**
    * get_seats method returns list of empty seats in theatre:theatre_id for the movie:movie_id
    * @params movie_id
    * @author Maksim Piriyev
    */
    const TheatreSeats get_seats(int movie_id, int theatre_id) {
        auto hall = mapTheatres[theatre_id]->hall(movie_id);
        if (hall) {
            return {movie_id, theatre_id, hall->empty_seats()};
        }
        return {movie_id, theatre_id, {}};
    }

    /**
    * get_bookings method returns all bookings in the system
    * @author Maksim Piriyev
    */

    const vector<shared_ptr<Booking>> get_bookings() {
        return vector<shared_ptr<Booking>>(bookings);
    }

    /**
    * get_bookings(client_id) method returns all bookings of the client in the system
    * @params client_id
    * @author Maksim Piriyev
    */
    const vector<shared_ptr<Booking>> get_bookings(int client_id) {
        vector<shared_ptr<Booking>> list;
        for (auto booking: bookings) {
            if (booking->client_id == client_id)
                list.push_back(booking);
        }
        return list;
    }

    /**
    * BookingEngine(movies, theatres) constructor initializes the system with the given movies and theatres
    * @params movies, theatres
    * @author Maksim Piriyev
    */
    BookingEngine(vector<shared_ptr<Movie>> movies, vector<shared_ptr<Theatre>> theatres) {
        for (auto theatre: theatres) {
            this->mapTheatres[theatre->id] = theatre;
            for (auto movie_id: theatre->get_movie_ids()) {
                this->mapMovieTheatres[movie_id].push_back(theatre);
            }
        }
        for (auto movie: movies) {
            this->mapMovies[movie->id] = movie;
        }
    }

    /**
    * book method books the seats for the client in the theatre for the movie and returns the booking
    * object, if the booking is successful, otherwise returns null.
    * It also adds the booking to the bookings list of the system.
    * @params client_id, theatre_id, movie_id, seat_ids
    * @author Maksim Piriyev
    */
    shared_ptr<Booking> book(int client_id, int theatre_id, int movie_id, vector<int> seat_ids) {
        auto theatre = mapTheatres[theatre_id];
        auto book_result = theatre->book(movie_id, seat_ids);
        if (book_result.is_booked) {
            auto booking = make_shared<Booking>(client_id, theatre_id, movie_id, book_result.hall_id, seat_ids, now());
            bookings.push_back(booking);
            return booking;
        }
        return nullptr;
    }
};
