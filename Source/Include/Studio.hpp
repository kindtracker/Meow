#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace std;

typedef enum { MyaOk, MyaFail } MyaReturnStatus;

class MyaStudio {
public:
  struct Color {
    Uint8 R, G, B, A;
  };

  int StartSDL();
  int StartStudio();
  int Close();
  int Frame();
  void DrawUI();

  TTF_Font *Font;
  SDL_Window *Window;
  SDL_Renderer *Renderer;
  bool Running;

private:
  void Text(SDL_Renderer *Renderer, float X, float Y, const char *String,
            MyaStudio::Color C);
};
