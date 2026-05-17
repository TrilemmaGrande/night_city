cmake_minimum_required(VERSION 3.20)
project(night_city)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXECUTABLE_SUFFIX ".html")

include(FetchContent)

# IMPORTANT: Web build settings FIRST
if (EMSCRIPTEN)
    set(PLATFORM Web CACHE STRING "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SUPPORT_X11 OFF CACHE BOOL "" FORCE)
    set(SUPPORT_WAYLAND OFF CACHE BOOL "" FORCE)
    set(SUPPORT_GLFW OFF CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)

add_executable(night_city
        main.cpp
        Config.cpp
        World.cpp
        Car.cpp
        Building.cpp
)
target_link_libraries(night_city raylib)

if (EMSCRIPTEN)
    target_compile_definitions(night_city PRIVATE PLATFORM_WEB)

    target_link_options(night_city PRIVATE
            "-sUSE_GLFW=3"
            "-sASYNCIFY"
            "-sALLOW_MEMORY_GROWTH"
            "-sWASM=1"
    )
endif()