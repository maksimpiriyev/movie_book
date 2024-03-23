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

/**
*   Current time as unix timespec
*/
static struct timespec now() {
    struct timespec spec = {0};
    clock_gettime(CLOCK_REALTIME, &spec);
    return spec;
}

/**
* @brief Movie class, has id and name
* @author Maksim Piriyev
*/

class Movie {
public:
    int id = 0;
    string name = "";

    Movie() {}

    Movie(int id, string name) : id(id), name(name) {}

    struct glaze {
        using T = Movie;
        static constexpr auto value = glz::object(
                &T::id,
                &T::name
        );
    };
};

/**
* @brief Client class, has id and name
* @author Maksim Piriyev
*/
class Client {
public:
    int id = 0;
    string name = "";

    Client() {}

    Client(int id, string name) : id(id), name(name) {}

    struct glaze {
        using T = Client;
        static constexpr auto value = glz::object(
                &T::id,
                &T::name
        );
    };

};

/**
* @brief Booking class, stores the booking information of a client, theatre, movie, hall, seats and timestamp of booking.
* @author Maksim Piriyev
*/
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
            struct timespec timestamp) : client_id(client_id), theatre_id(theatre_id), movie_id(movie_id),
                                         hall_id(hall_id),
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

/**
* @brief enum class for seat status, Free, Reserved, Booked
* @author Maksim Piriyev
*/
enum SeatStatus {
    Free = 0, Reserved, Booked
};

/**
* meta struct for serialization/deserialization operations
* @author Maksim Piriyev
*/
template<>
struct glz::meta<SeatStatus> {
    using
    enum SeatStatus;
    static constexpr auto value = enumerate(Free,
                                            Reserved,
                                            Booked
    );
};

/**
* @brief Hall class, has id, name, movie_id and seats. It has reserve and book methods to reserve and book seats. It has totalEmptySeats method to get the total empty seats in the hall. It has a semaphore to lock the hall while reserving or booking seats.
* @author Maksim Piriyev
*/

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

    /**
    * totalEmptySeats() method returns the total empty seats in the hall
    * @author Maksim Piriyev
    */
    int totalEmptySeats() {
        int emptySeatCount = 0;
        semaphore.acquire();
        for (int i = 0; i < sizeof(this->seats) / sizeof(SeatStatus); ++i) {
            if (this->seats[i] == SeatStatus::Free) {
                emptySeatCount++;
            }
        }
        semaphore.release();
        return emptySeatCount;
    }

    /**
    * empty_seats() method returns the list of empty seats in the hall
    * @author Maksim Piriyev
    */
    vector<int> empty_seats() {
        vector<int> seats;
        semaphore.acquire();
        for (int i = 0; i < sizeof(this->seats) / sizeof(SeatStatus); ++i) {
            if (this->seats[i] == SeatStatus::Free) {
                seats.push_back(i);
            }
        }
        semaphore.release();
        return seats;
    }

    /**
    * reserve(seats) method reserves the seats in the hall with the given seat numbers. It returns true if the seats are reserved successfully, otherwise false. It uses a semaphore to lock the hall while reserving the seats. It checks if the seats are free before reserving them. If any of the seats are not free, it releases the semaphore and returns false. Otherwise, it reserves the seats and releases the semaphore. It returns true if all the seats are reserved successfully, otherwise false.
    * @param seats vector of seat numbers to reserve in the hall
    * @author Maksim Piriyev
    */
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

    /**
    * book(seats) method books the seats in the hall with the given seat numbers. It returns true if the seats are booked successfully, otherwise false.It reserves the seats before booking them. If any of the seats are not reservable, it returns false. Otherwise, it books the seats. It returns true if all the seats are booked successfully, otherwise false.
    * @param seats vector of seat numbers to book in the hall
    * @author Maksim Piriyev
    */
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

/**
* @brief BookResult struct to store the result of the book operation,
* has hall_id and is_booked fields.
* @author Maksim Piriyev
*/
struct BookResult {
    int hall_id;
    bool is_booked;
};

/**
* @brief Theatre class, has id, name and halls.
* @author Maksim Piriyev
*/
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

    /**
    * get_movie_ids() method returns the list of movie ids presented in the halls
    * @author Maksim Piriyev
    */
    vector<int> get_movie_ids() {
        vector<int> movie_ids;
        for (auto hall: halls) {
            movie_ids.push_back(hall->movie_id);
        }
        return movie_ids;
    }

    /**
    * hall() method returns the hall with the given movie_id
    * @author Maksim Piriyev
    */
    shared_ptr<Hall> hall(int movie_id) {
        for (int i = 0; i < halls.size(); ++i) {
            if (halls[i]->movie_id == movie_id)
                return halls[i];
        }
        return nullptr;
    }

    /**
    * has_movie() method checks if the theatre has a movie with the given movie_id
    * @author Maksim Piriyev
    */
    bool has_movie(int movie_id) {
        return hall(movie_id) != nullptr;
    }

    /**
    * book() method books the seats in the hall with the given movie_id and
    * returns the hall_id and is_booked status in BookResult struct.
    * If the hall is not  found, it returns -1 as hall_id and false as is_booked status.
    * If the seats  are not available, it returns the hall_id and false as is_booked status.
    * If the seats are available, it reserves the seats and books them and returns the hall_id
    * and true as is_booked status in BookResult struct.
    * @param movie_id movie id to book the seats
    * @param seats vector of seat numbers to book in the hall
    * @return BookResult struct with hall_id and is_booked status
    * @author Maksim Piriyev
    */
    BookResult book(int movie_id, vector<int> seats) {
        auto hall = this->hall(movie_id);
        if (!hall)
            return {-1, false};

        auto result = hall->book(seats);
        return {hall->id, result};
    }
};
