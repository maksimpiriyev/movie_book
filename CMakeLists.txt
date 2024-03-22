cmake_minimum_required(VERSION 3.25)
project(movie_book)

set(CMAKE_CXX_STANDARD 20)

find_package(cereal REQUIRED)
find_package(uwebsockets REQUIRED)
find_package(Catch2 REQUIRED)
find_package(glaze REQUIRED)

configure_file(movies.json movies.json COPYONLY)
configure_file(theatres.json theatres.json COPYONLY)

add_executable(${PROJECT_NAME} main.cpp BookingEngine.cpp BookingEngine.h BookingLibrary.h BookingServer.cpp BookingServer.h tests.cpp)
target_link_libraries(${PROJECT_NAME} cereal::cereal)
target_link_libraries(${PROJECT_NAME} uwebsockets::uwebsockets)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain)
target_link_libraries(${PROJECT_NAME} glaze::glaze)

