cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME MyGame)

project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(${PROJECT_NAME} main.cc)

find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
IF(MINGW)
  target_link_libraries(${PROJECT_NAME} PRIVATE -lmingw32 -lSDL2main -lSDL2)
ENDIF(MINGW)
