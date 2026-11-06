#include <SDL3/SDL.h>

#include "Include/Studio.hpp"

int MyaStudio::StartSDL() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer("Mya", 640, 480, SDL_WINDOW_OPENGL, &Window,
                              &this->Renderer);

  return MyaOk;
}

int MyaStudio::StartStudio() {
  this->StartSDL();
  this->Running = true;
}

int MyaStudio::Close() {
  SDL_DestroyWindow(this->Window);
  SDL_Quit();
  return MyaOk;
}

int MyaStudio::Frame() {
  SDL_Event Event;
  while (SDL_PollEvent(&Event)) {
    if (Event.type == SDL_EVENT_QUIT) {
      Running = false;
      break;
    }
  }

  SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
  SDL_RenderClear(Renderer);
  SDL_RenderPresent(Renderer);

  return MyaOk;
}
