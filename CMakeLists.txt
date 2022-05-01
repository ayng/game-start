cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME MyGame)

project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(${PROJECT_NAME} main.cc)

configure_file(assets/SDL_logo.png assets/SDL_logo.png COPYONLY)
configure_file(assets/bloop.wav assets/bloop.wav COPYONLY)
configure_file(assets/Inconsolata.ttf assets/Inconsolata.ttf COPYONLY)

IF(EMSCRIPTEN)
  # To build with Emscripten, use the various hardcoded flags to pull in
  # ports. To list the available ports, run `emcc --show-ports`.
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL_IMAGE=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL_MIXER=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_VORBIS=1)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL_TTF=2)
  target_link_options(${PROJECT_NAME} PRIVATE --use-preload-plugins)
  target_link_options(${PROJECT_NAME} PRIVATE --preload-file assets)
  configure_file(index.html index.html COPYONLY)
ELSE()
  set(CMAKE_MODULE_PATH cmake/sdl2/)
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  find_package(SDL2_ttf REQUIRED)
  target_link_libraries(${PROJECT_NAME}
    SDL2::Main
    SDL2::Image
    SDL2::Mixer
    SDL2::TTF
  )

  # On Windows, if the -mconsole flag is not included, the program will not
  # print to stdout, nor will it respond to signals. Don't include this flag
  # when shipping.
  # TODO use a log file in both debug and release builds.
  IF(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
  ENDIF(WIN32)
ENDIF()
