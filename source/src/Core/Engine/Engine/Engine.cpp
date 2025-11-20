#include "Engine.h"
#include "Camera.h"
#include "ImGUIWindow.h"
#include "Physics.h"
#include "backends/imgui_impl_sdl2.h"
#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

// Constructors and Destructors
Engine::Engine() : m_Window(nullptr) {
  spdlog::info("[Engine] Engine instance created.");
}

Engine::~Engine() { spdlog::info("[Engine] Engine instance destroyed."); }

// Static Methods
Engine *Engine::getInstance() {
  static Engine instance;
  return &instance;
}

// Class Public Methods
void Engine::run() {
  spdlog::info("[Engine] Initiating shader game engine...");
  initEverything();

  if (m_Running) {
    spdlog::info("[Engine] Entering game loop...");
    gameLoop();
  } else {
    spdlog::error("[Engine] Initialization failed. Failed to enter game loop.");
  }
}

// Class Private Methods

void Engine::initEverything() {
  spdlog::info("[Engine] Initializing everything...");

  setOpenGLAttributes();

  m_Running = initSDL() && initWindow() && initOpenGLContext() && loadGLAD() &&
              initImGUIWindow() && initBulletPhysics();

  spdlog::info("[Engine] Initializing openGL Viewport...");
  initGLViewPort();
}

void Engine::setOpenGLAttributes() {
  spdlog::info("[Engine] Setting OpenGL Attributes...");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  spdlog::info("[Engine] OpenGL Attributes Setting Done.");
}

bool Engine::initSDL() {
  spdlog::info("[Engine] Initializing SDL...");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    spdlog::error("[Engine] SDL Failed to initialize: {}", SDL_GetError());
    return false;
  } else {
    spdlog::info("[Engine] SDL Initialized successfully.");
    return true;
  }
}

bool Engine::initWindow() {
  int initWindowWidth = 1600;
  int initWindowHeight = 920;

  m_Window = SDL_CreateWindow("Shader Game Engine", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, initWindowWidth,
                              initWindowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!m_Window) {
    spdlog::error("[Engine] Window creation failed: {}", SDL_GetError());
    return false;
  } else {
    spdlog::info("[Engine] Window created successfully.");

    m_WindowWidth = initWindowWidth;
    m_WindowHeight = initWindowHeight;
    return true;
  }
}

bool Engine::initOpenGLContext() {
  m_GLContext = SDL_GL_CreateContext(m_Window);
  if (!m_GLContext) {
    spdlog::error("[Engine] SDL_GL_CreateContext failed: {}", SDL_GetError());
    return false;
  }

  SDL_GL_SetSwapInterval(
      0); // Disable VSync: allows rendering at uncapped FPS instead of syncing
          // to monitor's refresh rate (e.g., 60Hz)
  spdlog::info("[Engine] SDL Context created successfully.");
  return true;
}

bool Engine::loadGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    spdlog::error("[Engine] Failed to initialize GLAD.");
    return false;
  }
  spdlog::info("[Engine] GLAD loaded successfully.");
  return true;
}

bool Engine::initImGUIWindow() {
  ImGUIWindow *imguiWindow = ImGUIWindow::getInstance();
  bool initSuccess = imguiWindow->init(m_Window, m_GLContext);
  if (!initSuccess) {
    spdlog::warn("[Engine] Failed to initialize ImGUIWindow.");
    return false;
  }
  return true;
}

bool Engine::initBulletPhysics() {
  if (!Physics::getInstance()->init()) {
    return false;
    spdlog::info("[Engine] Bullet Physics failed to initialize.");
  }

  return true;
}

void Engine::initGLViewPort() {
  glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

void Engine::gameLoop() {
  while (m_Running) {
    handleInput();
    update();
    render();
  }
  spdlog::info("[Engine] Engineloop terminated.");
  free();
}

void Engine::handleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT ||
        (event.type == SDL_WINDOWEVENT &&
         event.window.event == SDL_WINDOWEVENT_CLOSE &&
         event.window.windowID == SDL_GetWindowID(m_Window))) {
      spdlog::info("Detected window close!");
      m_Running = false;
      return;
    }

    if (event.type == SDL_KEYDOWN) {
      auto e_Key = event.key.keysym.sym;
      if ((e_Key == SDLK_LALT && e_Key == SDLK_F4) ||
          (e_Key == SDLK_RALT && e_Key == SDLK_F4)) {
        m_Running = false;
        return;
      }
    }

    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      m_WindowWidth = event.window.data1;
      m_WindowHeight = event.window.data2;
    }

    ImGui_ImplSDL2_ProcessEvent(&event);
  }
}

void Engine::update() {
  calculateDeltaTime();
  Physics::getInstance()->dynamicsWorld->stepSimulation(m_DeltaTime, 10);
}

void Engine::render() {
  glClearColor(1.0, 1.0, 1.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  ImGUIWindow::getInstance()->render();
  SDL_GL_SwapWindow(m_Window);
}

void Engine::calculateDeltaTime() {
  static Uint64 lastCounter = SDL_GetPerformanceCounter();
  Uint64 currentCounter = SDL_GetPerformanceCounter();

  m_DeltaTime = static_cast<float>(currentCounter - lastCounter) /
                static_cast<float>(SDL_GetPerformanceFrequency());

  lastCounter = currentCounter;
}

void Engine::free() {
  spdlog::info("Destroying engine resources...");
  Physics::getInstance()->free();
  ImGUIWindow::getInstance()->free();
  SDL_DestroyWindow(m_Window);
  SDL_GL_DeleteContext(m_GLContext);
  SDL_Quit();
  spdlog::info("Engine resources destroyed successfully.");
  spdlog::info("Shutting down spdlog...");
  spdlog::shutdown();
}
