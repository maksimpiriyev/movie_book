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

class BookingEngine {
    vector<shared_ptr<Booking>> bookings;

    map<int, shared_ptr<Movie>> mapMovies;
    map<int, shared_ptr<Theatre>> mapTheatres;
    map<int, shared_ptr<Client>> mapClients;
public:


    shared_ptr<Client> new_client(){
        auto id = mapClients.size()+1;
        stringstream stream;
        stream<<"Random Client "<<id;
        mapClients[id] = make_shared<Client>(id, stream.str());
        return mapClients[id];
    }
    const vector<shared_ptr<Movie>> get_movies(){
        vector<shared_ptr<Movie>> list;
        for (auto [_, movie] : mapMovies) {
            list.push_back(movie);
        }
        return list;
    }
    const vector<shared_ptr<Theatre>> get_theatres(){
        vector<shared_ptr<Theatre>> list;
        for (auto [_, theatre] : mapTheatres) {
            list.push_back(theatre);
        }
        return list;
    }
    const vector<shared_ptr<Theatre>> get_theatres(int movie_id){
        vector<shared_ptr<Theatre>> list;
        for (auto [ _, theatre] : mapTheatres) {
            if(theatre->has_movie(movie_id))
                list.push_back(theatre);
        }
        return list;
    }

    const vector<shared_ptr<Booking>> get_bookings(){
        return vector<shared_ptr<Booking>>(bookings);
    }

    const vector<shared_ptr<Booking>> get_bookings(int client_id){
        vector<shared_ptr<Booking>> list;
        for (auto booking : bookings) {
            if(booking->client_id == client_id)
                list.push_back(booking);
        }
        return list;
    }

    BookingEngine(vector<shared_ptr<Movie>> movies, vector<shared_ptr<Theatre>> theatres){
        for (auto item : movies) {
            this->mapMovies[item->id] = item;
        }
        for (auto item : theatres) {
            this->mapTheatres[item->id] = item;
        }
    }

    shared_ptr<Booking> book(int client_id, int theatre_id, int movie_id, vector<int> seat_ids){
        auto theatre = mapTheatres[theatre_id];
        auto book_result = theatre->book(movie_id, seat_ids);
        if(book_result.is_booked){
            auto booking = make_shared<Booking>(client_id, theatre_id, movie_id, book_result.hall_id, seat_ids, now());
            bookings.push_back(booking);
            return booking;
        }
        return nullptr;
    }
};
