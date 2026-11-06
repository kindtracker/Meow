#include <SDL3/SDL.h>
#include <stdio.h>

int main(int Argc, char *Argv[]) {
  SDL_Window *Window;
  bool Running = true;

  SDL_Init(SDL_INIT_VIDEO);

  Window = SDL_CreateWindow("Mya", 640, 480, SDL_WINDOW_OPENGL);

  while (Running) {
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_EVENT_QUIT) {
        Running = false;
        break;
      }
    }
  }

  SDL_DestroyWindow(Window);
  SDL_Quit();
  return 0;
}
