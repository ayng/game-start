#include <iostream>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

bool mainLoop(double dt) {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return true;
  }
  return false;
}


#ifdef __EMSCRIPTEN__
EM_BOOL emLoop(double time, void *userData) {
  // This function-local static variable is initialized exactly once --
  // when control first passes through its declaration -- i.e. subsequent
  // calls to this function will not reset this value.
  static double prevTime = 0;

  const double dt = time - prevTime;
  prevTime = time;

  const bool shouldQuit = mainLoop(dt);

  return shouldQuit ? EM_TRUE : EM_FALSE;
}
#endif

int main(int argc, char **argv) {

  SDL_Window* window;
  SDL_Renderer* renderer;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
    std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer)) {
    std::cout << "Couldn't create window and renderer: " << SDL_GetError() << std::endl;
    return 1;
  }

#ifdef __EMSCRIPTEN__
  emscripten_request_animation_frame_loop(emLoop, nullptr);
#else
  double dt;
  double prevTicks = 0;
  bool shouldQuit = false;

  while (!shouldQuit) {
    double ticks = SDL_GetTicks();
    dt = ticks - prevTicks;
    prevTicks = ticks;
    shouldQuit = mainLoop(dt);
    SDL_Delay(17);
  }
#endif

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
