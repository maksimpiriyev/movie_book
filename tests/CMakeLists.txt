#cmake_minimum_required(VERSION 3.25)
project(movie_book_tests)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)

configure_file(../movies.json movies.json COPYONLY)
configure_file(../theatres.json theatres.json COPYONLY)

add_executable(${PROJECT_NAME} tests.cpp )
target_link_libraries(${PROJECT_NAME} PRIVATE movie_book_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(${PROJECT_NAME})