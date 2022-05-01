#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

const int kViewportWidth = 640;
const int kViewportHeight = 480;

class GameData {
 public:
  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Texture* texture;
  SDL_Rect rect;

  Mix_Chunk* sfx_placeholder;

  TTF_Font* font;
  SDL_Texture* textTexture;
  SDL_Rect textRect;

  bool is_mouse_down;
};

bool mainLoop(double dt, GameData& g) {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return false;
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    Mix_PlayChannel(-1, g.sfx_placeholder, 0);
    g.is_mouse_down = true;
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    g.is_mouse_down = false;
  }

  g.rect.x = kViewportWidth / 2 - g.rect.w / 2;
  g.rect.y = kViewportHeight / 2 - g.rect.h / 2;
  if (g.is_mouse_down) {
    g.rect.y += 12;
  }

  g.textRect.x = kViewportWidth / 2 - g.textRect.w / 2;
  g.textRect.y = kViewportHeight / 2 - g.textRect.h / 2 - g.rect.h / 2 - 64;

  SDL_RenderClear(g.renderer);

  SDL_RenderCopy(g.renderer, g.texture, nullptr, &g.rect);
  SDL_RenderCopy(g.renderer, g.textTexture, nullptr, &g.textRect);
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

  if (SDL_CreateWindowAndRenderer(kViewportWidth, kViewportHeight, 0, &gameData.window, &gameData.renderer)) {
    std::cout << "Couldn't create window and renderer: " << SDL_GetError() << std::endl;
    return 1;
  }

  if (TTF_Init() == -1) {
    std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
  }

  {
    SDL_Surface* surface = IMG_Load("assets/SDL_logo.png");
    if (!surface) {
      std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
      return 1;
    }
    SDL_GetClipRect(surface, &gameData.rect);
    gameData.texture = SDL_CreateTextureFromSurface(gameData.renderer, surface);
    SDL_FreeSurface(surface);
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    std::cout << "Mix_OpenAudio: " << Mix_GetError() << std::endl;
    return 1;
  }

  gameData.sfx_placeholder = Mix_LoadWAV("assets/bloop.wav");
  if (!gameData.sfx_placeholder) {
    std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
  }

  gameData.font = TTF_OpenFont("assets/Inconsolata.ttf", 48);
  if (!gameData.font) {
    std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
  }

  {
    SDL_Surface* surface = TTF_RenderText_Shaded(gameData.font, "Click here!",
        {255, 255, 255, 255}, {0, 0, 0, 255});
    SDL_GetClipRect(surface, &gameData.textRect);
    gameData.textTexture = SDL_CreateTextureFromSurface(gameData.renderer,
        surface);
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

  TTF_CloseFont(gameData.font);

  Mix_CloseAudio();
  Mix_FreeChunk(gameData.sfx_placeholder);

  SDL_DestroyRenderer(gameData.renderer);
  SDL_DestroyWindow(gameData.window);

  SDL_Quit();
#endif

  return 0;
}
