#pragma once
#include <SDL2/SDL.h>

class Engine {
  // Constructors & Destructors
public:
  Engine();
  Engine(Engine &&) = default;
  Engine(const Engine &) = default;
  Engine &operator=(Engine &&) = default;
  Engine &operator=(const Engine &) = default;
  ~Engine();

  // Static Methods
public:
  static Engine *getInstance();

  // Class Private Attributes
  SDL_Window *m_Window;
  SDL_GLContext m_GLContext;
  bool m_Running;
  float m_DeltaTime;
  int m_WindowWidth;
  int m_WindowHeight;

  // Class Public Methods
public:
  void run();

  // Class Private Methods
private:
  // Initializers
  void initEverything();
  bool initLoggers();
  void setOpenGLAttributes();
  bool initSDL();
  bool initWindow();
  bool initOpenGLContext();
  bool loadGLAD();
  bool initUI();
  bool initPhysics();
  void initGLViewPort();

  // Engine Loop
  void gameLoop();

  void handleInput();
  void update();
  void render();

  // Others
  void calculateDeltaTime();
  void free();
};
