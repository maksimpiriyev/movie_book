//
// Created by Maksim Piriyev on 19.03.24.
//

#include <catch2/catch_test_macros.hpp>

#include "BookingEngine.h"

TEST_CASE("SerializationTest", "[movie_book_tests]"){  // 12/2/2020 -> 737761
    auto text = glz::write_json(Movie(2, "Movie2"));
    CHECK(text == "{\"id\":2,\"name\":\"Movie2\"}");
}

TEST_CASE("DeserializationTest", "[movie_book_tests]"){  // 12/0/2020 -> 0
    vector<shared_ptr<Movie>> movies;
    glz::read_file_json(movies, "movies.json", std::string{});
    REQUIRE(movies.size() == 2);

    vector<shared_ptr<Theatre>> theatres;
    glz::read_file_json(theatres, "theatres.json", std::string{});
    REQUIRE(theatres.size() == 2);
}

TEST_CASE("BookingSuccessTest", "[movie_book_tests]"){  // 12/0/2020 -> 0
    vector<shared_ptr<Movie>> movies;
    glz::read_file_json(movies, "movies.json", std::string{});
    vector<shared_ptr<Theatre>> theatres;
    glz::read_file_json(theatres, "theatres.json", std::string{});

    auto bookingEngine = new BookingEngine(movies, theatres);
    auto client = bookingEngine->new_client();
    auto result = bookingEngine->book(client->id, 1, 1,
                                      { 1, 2});
    REQUIRE(result != nullptr);
    REQUIRE(result->seat_ids.size() == 2);
}

TEST_CASE("BookingFailureTest", "[movie_book_tests]"){  // 12/0/2020 -> 0

    vector<shared_ptr<Movie>> movies;
    glz::read_file_json(movies, "movies.json", std::string{});
    vector<shared_ptr<Theatre>> theatres;
    glz::read_file_json(theatres, "theatres.json", std::string{});

    auto bookingEngine = new BookingEngine(movies, theatres);
    auto client = bookingEngine->new_client();
    bookingEngine->book(client->id, 1, 1,
                                      { 1, 2});
    auto result = bookingEngine->book(client->id, 1, 1,
                                 { 1, 2});
    REQUIRE(result == nullptr);
}