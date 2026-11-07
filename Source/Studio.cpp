#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Include/Studio.hpp"

int MyaStudio::StartSDL() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_CreateWindowAndRenderer("Mya", 640, 480,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, &Window,
                              &this->Renderer);
  SDL_SetRenderVSync(this->Renderer, 1);

  return MyaOk;
}

int MyaStudio::StartStudio() {
  int Result = this->StartSDL();
  if (Result != MyaOk) {
    return Result;
  }

  this->Font = TTF_OpenFont("Assets/NotoSans.ttf", 14.0f);

  this->Running = true;
  return MyaOk;
}

int MyaStudio::Close() {
  TTF_CloseFont(this->Font);
  TTF_Quit();

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
