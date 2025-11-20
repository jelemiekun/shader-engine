#pragma once
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

  bool init(SDL_Window *window, SDL_GLContext glContext) const;
  bool initImGuiWindowRenderSpace(const int &width, const int &height);
  void resizeFramebuffer(const int &width, const int &height);

  void createRootDockSpace();
  void resetLayout();
  void render();
  void free();
};
