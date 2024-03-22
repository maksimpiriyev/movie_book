cmake_minimum_required(VERSION 3.25)
project(movie_book)

set(CMAKE_CXX_STANDARD 20)

configure_file(movies.json movies.json COPYONLY)
configure_file(theatres.json theatres.json COPYONLY)

include_directories(lib)
add_subdirectory(lib)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} movie_book_lib)

enable_testing()
add_subdirectory(tests)