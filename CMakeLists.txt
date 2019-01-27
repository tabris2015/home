cmake_minimum_required(VERSION 3.7) # FetchContent is available in 3.11+
project(home)
include_directories(include)

find_package(raylib 2.0 REQUIRED) # Let CMake search for a raylib-config.cmake

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
