#include <iostream>

#include "SDL.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#ifdef __EMSCRIPTEN__
EM_BOOL
#else
bool
#endif
one_iter(double time, void *userData)
{
  // TODO render here
  std::cout << "time: " << time << std::endl;

#ifdef __EMSCRIPTEN__
  // Return true to keep the loop running.
  return EM_TRUE;
#else
  return true;
#endif
}

int main(int argc, char **argv) {
#ifdef __EMSCRIPTEN__
  // Receives a function to call and some user data to provide it.
  emscripten_request_animation_frame_loop(one_iter, nullptr);
#else
  double ticks;
  while (true) {
    ticks = SDL_GetTicks();
    one_iter(ticks, nullptr);
    // Delay to keep frame rate constant.
    SDL_Delay(17);
  }
#endif
}
