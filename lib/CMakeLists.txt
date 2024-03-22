cmake_minimum_required(VERSION 3.25)
project(movie_book_lib)

set(CMAKE_CXX_STANDARD 20)

find_package(cereal REQUIRED)
find_package(uwebsockets REQUIRED)
find_package(Catch2 REQUIRED)
find_package(glaze REQUIRED)

set(HEADER_FILES BookingEngine.h BookingLibrary.h BookingServer.h)

set(SOURCE_FILES BookingEngine.cpp BookingServer.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} cereal::cereal)
target_link_libraries(${PROJECT_NAME} uwebsockets::uwebsockets)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain)
target_link_libraries(${PROJECT_NAME} glaze::glaze)