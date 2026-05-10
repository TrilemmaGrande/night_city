cmake_minimum_required(VERSION 4.2)
project(night_city)

set(CMAKE_CXX_STANDARD 20)
find_package(raylib REQUIRED)
add_executable(night_city main.cpp)
target_link_libraries(night_city PRIVATE raylib)
