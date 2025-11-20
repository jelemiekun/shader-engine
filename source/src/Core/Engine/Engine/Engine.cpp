#include "Engine.h"
#include "Logger.h"
#include "Physics.h"
#include "UI.h"
#include "backends/imgui_impl_sdl2.h"
#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <glad/glad.h>

static UI *ui = UI::getInstance();
static Physics *physics = Physics::getInstance();

// Constructors and Destructors
Engine::Engine() : m_Window(nullptr) {
  Logger::engine->info("Engine instance created.");
}

Engine::~Engine() {
  Logger::engine->info("Engine instance destroyed.");
  Logger::engine->info("Program terminated.");
  Logger::free();
}

// Static Methods
Engine *Engine::getInstance() {
  static Engine instance;
  return &instance;
}

// Class Public Methods
void Engine::run() {
  Logger::engine->info("Initializing shader game engine...");
  initEverything();

  if (m_Running) {
    Logger::engine->info("Entering game loop...");
    gameLoop();
  } else {
    Logger::engine->error("Failed to initialize shader game engine.");
    Logger::engine->error("Program terminated.");
    Logger::free();
  }
}

// Class Private Methods

void Engine::initEverything() {
  Logger::engine->info("Initializing everything...");

  setOpenGLAttributes();

  m_Running = initSDL() && initWindow() && initOpenGLContext() && loadGLAD() &&
              initUI() && initPhysics();

  initGLViewPort();
}

void Engine::setOpenGLAttributes() {
  Logger::engine->info("Setting OpenGL attributes...");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  Logger::engine->info("Successfully set OpenGL attributes.");
}

bool Engine::initSDL() {
  Logger::engine->info("Initializing SDL...");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    Logger::engine->error("Failed to initialize SDL: {}", SDL_GetError());
    return false;
  } else {
    Logger::engine->info("Successfully initialized SDL.");
    return true;
  }
}

bool Engine::initWindow() {
  int initWindowWidth = 1600;
  int initWindowHeight = 920;

  Logger::engine->info("Initializing window with dimension {}x{}...",
                       initWindowWidth, initWindowHeight);

  m_Window = SDL_CreateWindow("Shader Game Engine", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, initWindowWidth,
                              initWindowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!m_Window) {
    Logger::engine->error("Failed to initialize window: {}", SDL_GetError());
    return false;
  } else {
    Logger::engine->info("Successfully initialized window.");

    m_WindowWidth = initWindowWidth;
    m_WindowHeight = initWindowHeight;
    return true;
  }
}

bool Engine::initOpenGLContext() {
  Logger::engine->info("Initializing OpenGL context...");

  m_GLContext = SDL_GL_CreateContext(m_Window);
  if (!m_GLContext) {
    Logger::engine->error("Failed to initialize SDL_GL context: {}",
                          SDL_GetError());
    return false;
  }

  SDL_GL_SetSwapInterval(
      0); // Disable VSync: allows rendering at uncapped FPS instead of syncing
          // to monitor's refresh rate (e.g., 60Hz)
  Logger::engine->info("Successfully initialized SDL_GL context.");
  return true;
}

bool Engine::loadGLAD() {
  Logger::engine->info("Loading GLAD...");

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    Logger::engine->error("Failed to initialize GLAD.");
    return false;
  }
  Logger::engine->info("Successfully loaded GLAD.");
  return true;
}

bool Engine::initUI() {
  Logger::engine->info("Initializing UI...");

  bool initSuccess = ui->init(m_Window, m_GLContext);
  if (!initSuccess) {
    Logger::engine->warn("Failed to initialize UI.");
    return false;
  }

  Logger::engine->info("Successfully initialized UI.");
  return true;
}

bool Engine::initPhysics() {
  Logger::engine->info("Initializing Physics...");

  if (!physics->init()) {
    return false;
    Logger::engine->info("Failed to initialize Physics.");
  }

  Logger::engine->info("Successfully initialized Physics.");
  return true;
}

void Engine::initGLViewPort() {
  Logger::engine->info("Initializing OpenGL viewport...");
  glViewport(0, 0, m_WindowWidth, m_WindowHeight);
  Logger::engine->info("Successfully initialized OpenGL viewport.");
}

void Engine::gameLoop() {
  while (m_Running) {
    handleInput();
    update();
    render();
  }
  Logger::engine->info("Engine game loop terminated.");
  free();
}

void Engine::handleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT ||
        (event.type == SDL_WINDOWEVENT &&
         event.window.event == SDL_WINDOWEVENT_CLOSE &&
         event.window.windowID == SDL_GetWindowID(m_Window))) {
      Logger::engine->info("Detected window close!");
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
  physics->dynamicsWorld->stepSimulation(m_DeltaTime, 10);
}

void Engine::render() {
  glClearColor(1.0, 1.0, 1.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  ui->render();
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
  Logger::engine->info("Destroying engine resources...");
  physics->free();
  ui->free();
  SDL_DestroyWindow(m_Window);
  SDL_GL_DeleteContext(m_GLContext);
  SDL_Quit();
  Logger::engine->info("Successfully destroyed Shader game engine resources.");
}
