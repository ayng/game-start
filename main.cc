#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

class GameData {
 public:
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
};

bool mainLoop(double dt, GameData& g) {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return false;
  }

  SDL_RenderCopy(g.renderer, g.texture, NULL, NULL);
  SDL_RenderPresent(g.renderer);

  return true;
}


#ifdef __EMSCRIPTEN__
EM_BOOL emLoop(double time, void* gameData) {
  // This function-local static variable is initialized exactly once during the
  // entire execution of the program -- when control first passes through its
  // declaration -- i.e. subsequent calls to this function will not reset this
  // value.
  static double prevTime = 0;

  const double dt = time - prevTime;
  prevTime = time;

  const bool shouldContinue = mainLoop(dt, *((GameData*) gameData));

  return shouldContinue ? EM_TRUE : EM_FALSE;
}
#endif

int main(int argc, char **argv) {

  static GameData gameData;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
    std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  if (SDL_CreateWindowAndRenderer(640, 480, 0, &gameData.window, &gameData.renderer)) {
    std::cout << "Couldn't create window and renderer: " << SDL_GetError() << std::endl;
    return 1;
  }

  {
    SDL_Surface* surface = IMG_Load("SDL_logo.png");
    if (!surface) {
      std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
      return 1;
    }
    gameData.texture = SDL_CreateTextureFromSurface(gameData.renderer, surface);
    SDL_FreeSurface(surface);
  }

#ifdef __EMSCRIPTEN__
  emscripten_request_animation_frame_loop(emLoop, (void*) &gameData);
#else
  double dt;
  double prevTicks = 0;
  bool shouldContinue = true;

  while (shouldContinue) {
    double ticks = SDL_GetTicks();
    dt = ticks - prevTicks;
    prevTicks = ticks;
    shouldContinue = mainLoop(dt, gameData);
    SDL_Delay(17);
  }

  SDL_DestroyRenderer(gameData.renderer);
  SDL_DestroyWindow(gameData.window);

  SDL_Quit();
#endif

  return 0;
}
