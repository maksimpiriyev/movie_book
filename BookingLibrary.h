//
// Created by Maksim Piriyev on 18.03.24.
//

#pragma once

#include <vector>
#include <set>
#include <chrono>
#include <semaphore>
#include <glaze/glaze.hpp>

using namespace std;
#define MAX_SEAT_COUNT 20

static struct timespec now(){
    struct timespec spec = {0};
    clock_gettime(CLOCK_REALTIME, &spec);
    return spec;
}

class Movie {
public:
    int id = 0;
    string name = "";

    Movie(){}
    Movie(int id, string name):id(id), name(name){}
    struct glaze {
        using T = Movie;
        static constexpr auto value = glz::object(
                &T::id,
                &T::name
        );
    };
};

class Client {
public:
    int id = 0;
    string name = "";

    Client(){}
    Client(int id, string name):id(id), name(name){}
    struct glaze {
        using T = Client;
        static constexpr auto value = glz::object(
                &T::id,
                &T::name
        );
    };

};

class Booking {
public:
    int client_id;
    int theatre_id;
    int movie_id;
    int hall_id;
    vector<int> seat_ids;
    struct timespec timestamp;

    Booking(int client_id,
            int theatre_id,
            int movie_id,
            int hall_id,
            vector<int> seat_ids,
            struct timespec timestamp) : client_id(client_id), theatre_id(theatre_id), movie_id(movie_id), hall_id(hall_id),
                                  seat_ids(seat_ids), timestamp(timestamp) {}

    struct glaze {
        using T = Booking;
        static constexpr auto value = glz::object(
                &T::client_id,
                &T::theatre_id,
                &T::movie_id,
                &T::hall_id,
                &T::seat_ids,
                &T::timestamp
        );
    };
};

enum SeatStatus {
    Free = 0, Reserved, Booked
};

template <>
struct glz::meta<SeatStatus> {
    using enum SeatStatus;
    static constexpr auto value = enumerate(Free,
                                            Reserved,
                                            Booked
    );
};

class Hall {
    binary_semaphore semaphore = binary_semaphore(1);
public:
    int id = 0;
    string name = "";
    int movie_id = 0;
    SeatStatus seats[MAX_SEAT_COUNT] = {SeatStatus::Free};

    struct glaze {
        using T = Hall;
        static constexpr auto value = glz::object(
                &T::id,
                &T::name,
                &T::movie_id
        );
    };

    int totalEmptySeats() {
        int emptySeatCount = 0;
        for (int i = 0; i < sizeof(seats) / sizeof(SeatStatus); ++i) {
            if (seats[i] == SeatStatus::Free) {
                emptySeatCount++;
            }
        }
        return emptySeatCount;
    }

    bool reserve(vector<int> seats) {
        semaphore.acquire();
        for (auto seat: seats) {
            if (this->seats[seat] != SeatStatus::Free) {
                semaphore.release();
                return false;
            }
        }
        for (auto seat: seats) {
            this->seats[seat] = SeatStatus::Reserved;
        }
        semaphore.release();
        return true;
    }

    bool book(vector<int> seats) {
        if (reserve(seats)) {
            for (auto seat: seats) {
                this->seats[seat] = SeatStatus::Booked;
            }
            return true;
        }
        return false;
    }

};

struct BookResult {
    int hall_id;
    bool is_booked;
};

class Theatre {
public:
    vector<shared_ptr<Hall>> halls;
    int id;
    string name;

    struct glaze {
        using T = Theatre;
        static constexpr auto value = glz::object(
                &T::id,
                &T::name,
                &T::halls
        );
    };

    shared_ptr<Hall> hall(int movie_id) {
        for (int i = 0; i < halls.size(); ++i) {
            if (halls[i]->movie_id == movie_id)
                return halls[i];
        }
        return nullptr;
    }

    bool has_movie(int movie_id) {
        return hall(movie_id) != nullptr;
    }

    BookResult book(int movie_id, vector<int> seats) {
        auto hall = this->hall(movie_id);
        auto result = hall->book(seats);
        return {hall->id, result};
    }
};
