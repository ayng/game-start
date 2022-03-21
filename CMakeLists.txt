cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME MyGame)

project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(${PROJECT_NAME} main.cc)

set(CMAKE_MODULE_PATH sdl2-cmake-modules/)

find_package(SDL2 REQUIRED)

target_link_libraries(${PROJECT_NAME} SDL2::Main)
