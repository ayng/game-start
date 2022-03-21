cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME MyGame)

project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(${PROJECT_NAME} main.cc)

set(CMAKE_MODULE_PATH cmake/sdl2/)

find_package(SDL2 REQUIRED)

target_link_libraries(${PROJECT_NAME} SDL2::Main)

# On Windows, if the -mconsole flag is not included, the program will not print
# to stdout, nor will it respond to signals.
IF(WIN32)
  target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
ENDIF(WIN32)
