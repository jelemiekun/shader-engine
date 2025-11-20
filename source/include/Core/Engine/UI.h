#pragma once
#include "UIVisibility.h"
#include <SDL.h>
#include <glad/glad.h>

class UI {
private:
  UI();

public:
  UI(const UI &) = delete;
  UI &operator=(const UI &) = delete;
  UI(UI &&) = delete;
  UI &operator=(UI &&) = delete;

  static UI *getInstance();

  static bool willResetLayout;
  static const char *rootDockSpace;
  static UIVisibility uiVisibility;

  bool init(SDL_Window *window, SDL_GLContext glContext) const;
  bool initImGuiWindowRenderSpace(const int &width, const int &height);
  void resizeFramebuffer(const int &width, const int &height);

  void createRootDockSpace();
  void createMainMenuBar();
  void resetLayout();
  void resetBitFieldsValues();
  void render();
  void renderImGuiWindows();
  void free();
};
