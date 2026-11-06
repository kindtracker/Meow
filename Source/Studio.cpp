#include <SDL3/SDL.h>

#define CLAY_IMPLEMENTATION
#include "Clay.h"
#include "ClayRenderer.c"

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
  return MyaOk;
}

int MyaStudio::Frame() {
  SDL_Event Event;

  while (SDL_PollEvent(&Event)) {
    if (Event.type == SDL_EVENT_QUIT) {
      Running = false;
    }
  }

  Clay_RenderCommandArray Commands = MyaStudio::CreateLayout();

  SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
  SDL_RenderClear(Renderer);

  SDL_Clay_RenderClayCommands(&ClayRenderer, &Commands);

  SDL_RenderPresent(Renderer);

  return MyaOk;
}

Clay_RenderCommandArray MyaStudio::CreateLayout() {
  Clay_BeginLayout();

  CLAY(CLAY_ID("MainPanel"),
       {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0),
                              .height = CLAY_SIZING_GROW(0)},
                   .padding = CLAY_PADDING_ALL(20),
                   .layoutDirection = CLAY_TOP_TO_BOTTOM,
                   .childGap = 10},
        .backgroundColor = {35, 35, 40, 255}}) {
    CLAY_TEXT(
        CLAY_STRING("Welcome to Mya Studio!"),
        CLAY_TEXT_CONFIG(
            {.fontId = 0, .fontSize = 24, .textColor = {255, 255, 255, 255}}));
  }

  return Clay_EndLayout();
}
