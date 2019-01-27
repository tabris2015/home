cmake_minimum_required(VERSION 3.7) # FetchContent is available in 3.11+
project(home)
include_directories(include)

find_package(raylib 2.0 QUIET) # Let CMake search for a raylib-config.cmake

if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

        # build raylib
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

    endif()
endif()
# This is the main part:

add_executable(${PROJECT_NAME} src/main.cpp)

set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib m)


add_executable(asteroids src/examples/asteroids.c)
#set(raylib_VERBOSE 1)
target_link_libraries(asteroids raylib m)


add_executable(survival src/examples/survival.c)
#set(raylib_VERBOSE 1)
target_link_libraries(survival raylib m)


add_executable(joystick src/examples/joystick.c)
#set(raylib_VERBOSE 1)
target_link_libraries(joystick raylib m)


add_executable(basic src/examples/basic.cpp)
#set(raylib_VERBOSE 1)
target_link_libraries(basic raylib m)
