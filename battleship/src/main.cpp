#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <ostream>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
  }

  SDL_Window *win =
      SDL_CreateWindow("test", 100, 100, 640, 480, SDL_WINDOW_OPENGL);

  if (win == nullptr) {
    std::cerr << "SDL Error: " << SDL_GetError() << "\n";
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    std::cerr << "SDL Renderer Error: " << SDL_GetError() << "\n";
  }

  // event loop
  SDL_SetRenderDrawColor(ren, 40, 40, 60, 255);
  SDL_RenderClear(ren);

  bool quit = false;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          quit = true;
        }
      }
    }

    SDL_RenderPresent(ren);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
