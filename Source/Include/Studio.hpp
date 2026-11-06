#pragma once
#include <SDL3/SDL.h>

using namespace std;

typedef enum { MyaOk, MyaFail } MyaReturnStatus;

class MyaStudio {
public:
  int StartSDL();
  int StartStudio();
  int Frame();
  int Close();

  SDL_Window *Window;
  SDL_Renderer *Renderer;
  bool Running;
};
