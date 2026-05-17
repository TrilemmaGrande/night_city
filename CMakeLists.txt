cmake_minimum_required(VERSION 4.2)
project(night_city)

set(CMAKE_CXX_STANDARD 20)
find_package(raylib REQUIRED)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.txt
${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(night_city main.cpp
        Building.cpp
        Building.h
        Car.cpp
        Car.h
        World.cpp
        World.h
        Config.cpp
        Config.h
)
target_link_libraries(night_city PRIVATE raylib)
