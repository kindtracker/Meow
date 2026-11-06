#pragma once
#include "Clay.h"
#include <SDL3/SDL.h>

using namespace std;

typedef enum { MyaOk, MyaFail } MyaReturnStatus;

class MyaStudio {
public:
  int StartSDL();
  int StartStudio();
  int Close();
  int Frame();
  Clay_RenderCommandArray CreateLayout();

  SDL_Window *Window;
  SDL_Renderer *Renderer;
  bool Running;
};
