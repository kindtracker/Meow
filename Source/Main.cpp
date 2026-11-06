#include "Include/Studio.hpp"

using namespace std;

int main(int Argc, char *Argv[]) {
  MyaStudio Studio;

  Studio.StartStudio();
  while (Studio.Running) {
    Studio.Frame();
  }
  return Studio.Close();
}
