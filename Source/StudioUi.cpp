#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>

#include "Include/Studio.hpp"

namespace StudioUI {

constexpr MyaStudio::Color Mantle = {24, 25, 38, 255};
constexpr MyaStudio::Color Base = {30, 32, 48, 255};
constexpr MyaStudio::Color Surface0 = {54, 58, 79, 255};
constexpr MyaStudio::Color Surface1 = {73, 77, 100, 255};
constexpr MyaStudio::Color Overlay0 = {91, 96, 120, 255};
constexpr MyaStudio::Color Overlay1 = {110, 115, 141, 255};

constexpr MyaStudio::Color TextColor = {202, 211, 245, 255};
constexpr MyaStudio::Color Subtext0 = {165, 173, 203, 255};
constexpr MyaStudio::Color Subtext1 = {184, 192, 224, 255};

constexpr MyaStudio::Color Blue = {138, 173, 244, 255};
constexpr MyaStudio::Color Lavender = {183, 189, 248, 255};
constexpr MyaStudio::Color Green = {166, 218, 149, 255};
constexpr MyaStudio::Color Yellow = {238, 212, 159, 255};
constexpr MyaStudio::Color Red = {237, 135, 150, 255};

TTF_Font *ActiveFont = nullptr;

void SetColor(SDL_Renderer *Renderer, MyaStudio::Color C) {
  SDL_SetRenderDrawColor(Renderer, C.R, C.G, C.B, C.A);
}

void Text(SDL_Renderer *Renderer, float X, float Y, const char *String,
          MyaStudio::Color C) {
  if (!ActiveFont || !String) {
    return;
  }

  SDL_Color SDLTextColor = {C.R, C.G, C.B, C.A};
  SDL_Surface *Surface =
      TTF_RenderText_Blended(ActiveFont, String, 0, SDLTextColor);

  if (!Surface) {
    SDL_Log("TTF_RenderText_Blended failed: %s", SDL_GetError());
    return;
  }

  SDL_Texture *Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
  if (Texture) {
    SDL_FRect Destination = {X, Y, static_cast<float>(Surface->w),
                             static_cast<float>(Surface->h)};
    SDL_RenderTexture(Renderer, Texture, nullptr, &Destination);
    SDL_DestroyTexture(Texture);
  } else {
    SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
  }

  SDL_DestroySurface(Surface);
}

void Fill(SDL_Renderer *Renderer, float X, float Y, float W, float H,
          MyaStudio::Color C) {
  SDL_FRect Rect = {X, Y, W, H};

  SetColor(Renderer, C);
  SDL_RenderFillRect(Renderer, &Rect);
}

void Border(SDL_Renderer *Renderer, float X, float Y, float W, float H,
            MyaStudio::Color C) {
  SDL_FRect Rect = {X, Y, W, H};

  SetColor(Renderer, C);
  SDL_RenderRect(Renderer, &Rect);
}

void Line(SDL_Renderer *Renderer, float X1, float Y1, float X2, float Y2,
          MyaStudio::Color C) {
  SetColor(Renderer, C);
  SDL_RenderLine(Renderer, X1, Y1, X2, Y2);
}

void FillRounded(SDL_Renderer *Renderer, float X, float Y, float W, float H,
                 float Radius, MyaStudio::Color C) {
  Radius = SDL_clamp(Radius, 0.0f, SDL_min(W, H) / 2.0f);

  if (Radius <= 0.0f) {
    Fill(Renderer, X, Y, W, H, C);
    return;
  }

  constexpr int Segments = 8;
  constexpr float PI = 3.14159265358979323846f;

  SDL_Vertex Vertices[4 * (Segments + 2)];

  int Count = 0;

  auto AddVertex = [&](float VX, float VY) {
    Vertices[Count++] = {
        {VX, VY},
        {static_cast<float>(C.R) / 255.0f, static_cast<float>(C.G) / 255.0f,
         static_cast<float>(C.B) / 255.0f, static_cast<float>(C.A) / 255.0f},
        {0.0f, 0.0f}};
  };

  const float CentersX[] = {X + W - Radius, X + W - Radius, X + Radius,
                            X + Radius};

  const float CentersY[] = {Y + Radius, Y + H - Radius, Y + H - Radius,
                            Y + Radius};

  const float StartAngles[] = {-PI / 2.0f, 0.0f, PI / 2.0f, PI};

  for (int Corner = 0; Corner < 4; Corner++) {
    AddVertex(CentersX[Corner], CentersY[Corner]);

    for (int i = 0; i <= Segments; i++) {
      float Angle = StartAngles[Corner] + (PI / 2.0f) * i / Segments;

      AddVertex(CentersX[Corner] + cosf(Angle) * Radius,
                CentersY[Corner] + sinf(Angle) * Radius);
    }
  }

  int Indices[4 * Segments * 3];
  int IndexCount = 0;

  for (int Corner = 0; Corner < 4; Corner++) {
    int Center = Corner * (Segments + 2);

    for (int i = 0; i < Segments; i++) {
      int A = Center;
      int B = Center + 1 + i;
      int D = Center + 1 + i + 1;

      Indices[IndexCount++] = A;
      Indices[IndexCount++] = B;
      Indices[IndexCount++] = D;
    }
  }

  SDL_RenderGeometry(Renderer, nullptr, Vertices, Count, Indices, IndexCount);
}

void DrawRounded(SDL_Renderer *Renderer, float X, float Y, float W, float H,
                 float Radius, MyaStudio::Color C) {
  constexpr int Segments = 12;
  constexpr float PI = 3.14159265358979323846f;

  Radius = SDL_clamp(Radius, 0.0f, SDL_min(W, H) / 2.0f);

  if (Radius <= 0.0f) {
    Border(Renderer, X, Y, W, H, C);
    return;
  }

  SDL_FPoint Points[4 * (Segments + 1) + 1];

  const float CentersX[] = {X + W - Radius, X + W - Radius, X + Radius,
                            X + Radius};

  const float CentersY[] = {Y + Radius, Y + H - Radius, Y + H - Radius,
                            Y + Radius};

  const float StartAngles[] = {-PI / 2.0f, 0.0f, PI / 2.0f, PI};

  int Count = 0;

  for (int Corner = 0; Corner < 4; Corner++) {
    for (int i = 0; i <= Segments; i++) {
      float Angle = StartAngles[Corner] + (PI / 2.0f) * i / Segments;

      Points[Count++] = {CentersX[Corner] + cosf(Angle) * Radius,
                         CentersY[Corner] + sinf(Angle) * Radius};
    }
  }

  Points[Count++] = Points[0];

  SetColor(Renderer, C);

  SDL_RenderLines(Renderer, Points, Count);
}

void DrawPanel(SDL_Renderer *Renderer, float X, float Y, float W, float H) {
  FillRounded(Renderer, X, Y, W, H, 6.0f, Surface0);
  DrawRounded(Renderer, X, Y, W, H, 6.0f, Overlay0);
}

void DrawPanelTitle(SDL_Renderer *Renderer, float X, float Y, float W,
                    const char *Title) {
  FillRounded(Renderer, X, Y, W, 28, 6.0f, Mantle);

  Line(Renderer, X, Y + 27, X + W, Y + 27, Surface1);

  Text(Renderer, X + 7, Y + 10, "|||", Overlay1);
  Text(Renderer, X + 34, Y + 10, Title, TextColor);
}

void DrawSearchBar(SDL_Renderer *Renderer, float X, float Y, float W,
                   const char *Placeholder) {
  Fill(Renderer, X + 8, Y + 8, W - 16, 28, Surface1);

  DrawRounded(Renderer, X + 8, Y + 8, W - 16, 28, 6.0f, Overlay0);

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
  FillRounded(Renderer, 0, 0, Width, 32, 6.0f, Surface0);
  DrawRounded(Renderer, 0, 0, Width, 32, 6.0f, Overlay0);

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

  FillRounded(Renderer, X + 1, Y + 29, W - 2, H - 30, 6.0f, Mantle);

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
  StudioUI::ActiveFont = this->Font;

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
