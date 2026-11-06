#include <SDL3/SDL.h>

#include "Include/Studio.hpp"

namespace StudioUI {

struct Color {
  Uint8 R, G, B, A;
};

constexpr Color Mantle = {24, 25, 38, 255};
constexpr Color Base = {30, 32, 48, 255};
constexpr Color Surface0 = {54, 58, 79, 255};
constexpr Color Surface1 = {73, 77, 100, 255};
constexpr Color Overlay0 = {91, 96, 120, 255};
constexpr Color Overlay1 = {110, 115, 141, 255};

constexpr Color TextColor = {202, 211, 245, 255};
constexpr Color Subtext0 = {165, 173, 203, 255};
constexpr Color Subtext1 = {184, 192, 224, 255};

constexpr Color Blue = {138, 173, 244, 255};
constexpr Color Lavender = {183, 189, 248, 255};
constexpr Color Green = {166, 218, 149, 255};
constexpr Color Yellow = {238, 212, 159, 255};
constexpr Color Red = {237, 135, 150, 255};

void SetColor(SDL_Renderer *Renderer, Color C) {
  SDL_SetRenderDrawColor(Renderer, C.R, C.G, C.B, C.A);
}

void Fill(SDL_Renderer *Renderer, float X, float Y, float W, float H, Color C) {
  SDL_FRect Rect = {X, Y, W, H};

  SetColor(Renderer, C);
  SDL_RenderFillRect(Renderer, &Rect);
}

void Border(SDL_Renderer *Renderer, float X, float Y, float W, float H,
            Color C) {
  SDL_FRect Rect = {X, Y, W, H};

  SetColor(Renderer, C);
  SDL_RenderRect(Renderer, &Rect);
}

void Line(SDL_Renderer *Renderer, float X1, float Y1, float X2, float Y2,
          Color C) {
  SetColor(Renderer, C);
  SDL_RenderLine(Renderer, X1, Y1, X2, Y2);
}

void Text(SDL_Renderer *Renderer, float X, float Y, const char *String,
          Color C) {
  SetColor(Renderer, C);
  SDL_RenderDebugText(Renderer, X, Y, String);
}

void DrawPanel(SDL_Renderer *Renderer, float X, float Y, float W, float H) {
  Fill(Renderer, X, Y, W, H, Surface0);
  Border(Renderer, X, Y, W, H, Overlay0);
}

void DrawPanelTitle(SDL_Renderer *Renderer, float X, float Y, float W,
                    const char *Title) {
  Fill(Renderer, X, Y, W, 28, Mantle);

  Line(Renderer, X, Y + 27, X + W, Y + 27, Surface1);

  Text(Renderer, X + 7, Y + 10, "|||", Overlay1);
  Text(Renderer, X + 34, Y + 10, Title, TextColor);
}

void DrawSearchBar(SDL_Renderer *Renderer, float X, float Y, float W,
                   const char *Placeholder) {
  Fill(Renderer, X + 8, Y + 8, W - 16, 28, Surface1);

  Border(Renderer, X + 8, Y + 8, W - 16, 28, Overlay0);

  Text(Renderer, X + 17, Y + 18, Placeholder, Subtext0);
}

void DrawToolbarButton(SDL_Renderer *Renderer, float X, float Y,
                       const char *Label, bool Active) {
  if (Active) {
    Fill(Renderer, X, Y, 28, 28, Surface1);
  }

  Text(Renderer, X + 9, Y + 10, Label, Active ? Green : TextColor);
}

void DrawToolbar(SDL_Renderer *Renderer, int Width) {
  Fill(Renderer, 0, 0, Width, 32, Surface0);
  Border(Renderer, 0, 0, Width, 32, Overlay0);

  Text(Renderer, 7, 11, "|||", Overlay1);

  DrawToolbarButton(Renderer, 28, 2, "C", true);
  DrawToolbarButton(Renderer, 60, 2, "M", false);
  DrawToolbarButton(Renderer, 92, 2, "S", false);
  DrawToolbarButton(Renderer, 124, 2, "R", false);

  Line(Renderer, 164, 7, 164, 25, Overlay0);

  Text(Renderer, 176, 11, "Move:", Subtext0);
  Text(Renderer, 224, 11, "1", TextColor);
  Text(Renderer, 242, 11, "Units", Subtext1);

  Text(Renderer, 294, 11, "Rotation:", Subtext0);
  Text(Renderer, 368, 11, "45", TextColor);
  Text(Renderer, 392, 11, "Degrees", Subtext1);

  Text(Renderer, Width - 250, 11, "Play", Green);
  Text(Renderer, Width - 200, 11, "Pause", Yellow);
  Text(Renderer, Width - 130, 11, "Stop", Red);

  Text(Renderer, Width - 85, 11, "kindtracker", TextColor);
  Text(Renderer, Width - 12, 11, "*", Lavender);
}

void DrawExplorer(SDL_Renderer *Renderer, float X, float Y, float W, float H) {
  DrawPanel(Renderer, X, Y, W, H);

  DrawPanelTitle(Renderer, X, Y, W, "Explorer");

  Text(Renderer, X + W - 65, Y + 10, "Toolbox", Subtext0);

  DrawSearchBar(Renderer, X, Y + 28, W, "Filter instances...");

  float ItemY = Y + 82;

  Text(Renderer, X + 12, ItemY, "v  Workspace", TextColor);

  ItemY += 24;
  Text(Renderer, X + 30, ItemY, "Camera", Subtext0);

  ItemY += 22;
  Text(Renderer, X + 30, ItemY, "Terrain", Subtext0);

  ItemY += 22;
  Text(Renderer, X + 30, ItemY, "Part", Green);

  ItemY += 22;
  Text(Renderer, X + 30, ItemY, "Folder", Subtext0);
}

void DrawProperties(SDL_Renderer *Renderer, float X, float Y, float W,
                    float H) {
  DrawPanel(Renderer, X, Y, W, H);

  DrawPanelTitle(Renderer, X, Y, W, "Properties");

  DrawSearchBar(Renderer, X, Y + 28, W, "Filter properties...");

  Text(Renderer, X + 12, Y + 90, "Select an instance", Subtext0);

  Text(Renderer, X + 12, Y + 106, "to view its properties.", Subtext0);
}

void DrawProject(SDL_Renderer *Renderer, float X, float Y, float W, float H) {
  DrawPanel(Renderer, X, Y, W, H);

  DrawPanelTitle(Renderer, X, Y, W, "Project Name");

  Fill(Renderer, X + 1, Y + 29, W - 2, H - 30, Mantle);

  Text(Renderer, X + W / 2 - 40, Y + H / 2, "Viewport", Overlay1);
}

void DrawConsole(SDL_Renderer *Renderer, float X, float Y, float W, float H) {
  DrawPanel(Renderer, X, Y, W, H);

  DrawPanelTitle(Renderer, X, Y, W, "Console");

  DrawSearchBar(Renderer, X, Y + 28, W, "Filter messages...");

  float ConfigY = Y + 75;

  Text(Renderer, X + 12, ConfigY, "Context", Overlay1);
  Text(Renderer, X + 85, ConfigY, "[ ] Client", TextColor);
  Text(Renderer, X + 170, ConfigY, "[ ] Server", TextColor);

  ConfigY += 20;

  Text(Renderer, X + 12, ConfigY, "Severity", Overlay1);
  Text(Renderer, X + 85, ConfigY, "[x] Info", Blue);
  Text(Renderer, X + 170, ConfigY, "[x] Warning", Yellow);
  Text(Renderer, X + 285, ConfigY, "[x] Error", Red);

  ConfigY += 25;

  Line(Renderer, X + 4, ConfigY, X + W - 4, ConfigY, Surface1);

  ConfigY += 12;

  Text(Renderer, X + 12, ConfigY, "[Info] Studio initialized.", Green);

  ConfigY += 16;

  Text(Renderer, X + 12, ConfigY, "[Info] Ready.", Subtext0);
}

} // namespace StudioUI

void MyaStudio::DrawUI() {
  int Width, Height;

  SDL_GetRenderOutputSize(this->Renderer, &Width, &Height);

  StudioUI::Fill(this->Renderer, 0, 0, Width, Height, StudioUI::Mantle);

  constexpr float ToolbarHeight = 32;
  constexpr float Gap = 4;
  constexpr float ConsoleHeight = 275;

  const float LeftWidth = Width * 0.20f;

  const float ContentY = ToolbarHeight + Gap;

  const float LeftHeight = Height - ToolbarHeight - Gap * 2;

  const float ExplorerHeight = (LeftHeight - Gap) / 2.0f;

  const float PropertiesY = ContentY + ExplorerHeight + Gap;

  const float PropertiesHeight = LeftHeight - ExplorerHeight - Gap;

  const float RightX = LeftWidth + Gap;

  const float RightWidth = Width - RightX - Gap;

  const float ProjectHeight = Height - ConsoleHeight - ToolbarHeight - Gap * 3;

  const float ConsoleY = Height - ConsoleHeight;

  StudioUI::DrawToolbar(this->Renderer, Width);

  StudioUI::DrawExplorer(this->Renderer, 0, ContentY, LeftWidth,
                         ExplorerHeight);

  StudioUI::DrawProperties(this->Renderer, 0, PropertiesY, LeftWidth,
                           PropertiesHeight);

  StudioUI::DrawProject(this->Renderer, RightX, ContentY, RightWidth,
                        ProjectHeight);

  StudioUI::DrawConsole(this->Renderer, RightX, ConsoleY, RightWidth,
                        ConsoleHeight);
}
