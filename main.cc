#include <iostream>

#include "SDL.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

void mainLoop(double dt) {
  std::cout << dt << std::endl;
}


#ifdef __EMSCRIPTEN__
EM_BOOL emLoop(double time, void *userData) {
  // This function-local static variable is initialized exactly once --
  // when control first passes through its declaration -- i.e. subsequent
  // calls to this function will not reset this value.
  static double prevTime = 0;

  double dt = time - prevTime;
  prevTime = time;

  mainLoop(dt);

  return EM_TRUE;
}
#endif

int main(int argc, char **argv) {

#ifdef __EMSCRIPTEN__

  emscripten_request_animation_frame_loop(emLoop, nullptr);

#else

  double dt;
  double prevTicks = 0;

  while (true) {

    double ticks = SDL_GetTicks();
    dt = ticks - prevTicks;
    prevTicks = ticks;

    mainLoop(dt);

    SDL_Delay(17);
  }

#endif

}
