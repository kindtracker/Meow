#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "Include/Studio.hpp"

int MyaStudio::StartSDL() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer("Mya", 640, 480, SDL_WINDOW_OPENGL, &Window,
                              &this->Renderer);

  return MyaOk;
}

int MyaStudio::StartStudio() {
  int Result = this->StartSDL();
  if (Result != MyaOk) {
    return Result;
  }

  this->Running = true;
  return MyaOk;
}

int MyaStudio::Close() {
  SDL_DestroyRenderer(this->Renderer);
  SDL_DestroyWindow(this->Window);
  SDL_Quit();
  return MyaOk;
}

int MyaStudio::Frame() {
  SDL_Event Event;

  while (SDL_PollEvent(&Event)) {
    if (Event.type == SDL_EVENT_QUIT) {
      Running = false;
    }
  }

  SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
  SDL_RenderClear(Renderer);
  this->DrawUI();
  SDL_RenderPresent(Renderer);

  return MyaOk;
}
