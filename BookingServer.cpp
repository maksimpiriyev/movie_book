//
// Created by Maksim Piriyev on 19.03.24.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <map>
#include <array>
#include "App.h"
#include "BookingEngine.h"
#include "BookingServer.h"

using namespace std;

struct BookRequest {
    int theatre_id;
    int movie_id;
    vector<int> seats;
};

void RunServer() {
    vector<shared_ptr<Movie>> movies;
    glz::read_file_json(movies, "movies.json", std::string{});
    vector<shared_ptr<Theatre>> theatres;
    glz::read_file_json(theatres, "mapTheatres.json", std::string{});

    auto bookingEngine = new BookingEngine(movies, theatres);

    auto app = uWS::App();
    app.get("/list/movies", [&](auto *res, auto *req) {
        res->writeStatus("200 OK");
        res->writeHeader("Content-Type", "application/json; charset=utf-8");
        res->write(glz::write_json(bookingEngine->get_movies()));
        res->end();
    });

    app.get("/list/mapTheatres", [&](auto *res, auto *req) {
        res->writeStatus("200 OK");
        res->writeHeader("Content-Type", "application/json; charset=utf-8");
        res->write(glz::write_json(bookingEngine->get_theatres()));
        res->end();
    });

    app.get("/list/mapTheatres/:movie_id", [&](auto *res, auto *req) {
        res->writeStatus("200 OK");
        res->writeHeader("Content-Type", "application/json; charset=utf-8");
        res->write(glz::write_json(bookingEngine->get_theatres(stoi(string(req->getParameter("movie_id"))))));
        res->end();
    });

    app.get("/list/bookings/:client_id", [&](auto *res, auto *req) {
        res->writeStatus("200 OK");
        res->writeHeader("Content-Type", "application/json; charset=utf-8");
        res->write(glz::write_json(bookingEngine->get_bookings(0)));
        res->end();
    });

    app.get("/list/bookings", [&](auto *res, auto *req) {
        res->writeStatus("200 OK");
        res->writeHeader("Content-Type", "application/json; charset=utf-8");
        res->write(glz::write_json(bookingEngine->get_bookings()));
        res->end();
    });

    app.post("/book", [&](auto *res, auto *req) {
        auto bodyBuffer = make_shared<string>();
        res->onData([&, res, bodyBuffer](std::string_view chunk, bool isLast) mutable {
            if (isLast) {
                bodyBuffer->append(chunk);
                auto book_request_result = glz::read_json<BookRequest>(*bodyBuffer);
                if (book_request_result) {
                    auto book_request = book_request_result.value();
                    auto result = bookingEngine->book(0, book_request.theatre_id, book_request.movie_id,
                                                      book_request.seats);
                    if (result) {
                        res->end(glz::write_json(*result));
                    } else {
                        res->end("{ \"result\": false }}");
                    }
                }
            } else {
                bodyBuffer->append(chunk);
            }


        });

        res->onAborted([]() {});
    });

    app.listen(9001, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        } else {
            std::cout << "Failed to load or to bind to port" << std::endl;
        }
    });

    app.run();
}