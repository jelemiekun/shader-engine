#include "UI.h"
#include "Logger.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "nfd.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>

const static constexpr char *OPENGL_VERSION = "#version 410";

bool UI::willResetLayout = true;
const char *UI::rootDockSpace = "RootDockSpace";
UIVisibility UI::uiVisibility;

UI::UI() {}

UI *UI::getInstance() {
  static UI instance;
  return &instance;
}

bool UI::init(SDL_Window *window, SDL_GLContext glContext) const {
  Logger::ui->info("Initializing ImGui...");
  bool initSuccess = true;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    // TODO: gawin 'tong alpha value dynamic
    style.Colors[ImGuiCol_WindowBg].w = 0.2f;
  }

  // Setup Platform/Renderer backends
  if (!ImGui_ImplSDL2_InitForOpenGL(window, glContext)) {
    Logger::ui->error("Failed to initialize ImGui SDL2 backend.");
    initSuccess = false;
  }

  Logger::ui->trace("Successfully initialized ImGui SDL2 backend.");

  if (!ImGui_ImplOpenGL3_Init(OPENGL_VERSION)) {
    Logger::ui->error("Failed to initialize ImGui OpenGL3 backend.");
    initSuccess = false;
  }

  Logger::ui->trace("Successfully initialized ImGUi OpenGL3 backend.");

  Logger::ui->info("Successfully initialized ImGui.");
  return initSuccess;
}

void UI::createRootDockSpace() {
  ImGuiIO &io = ImGui::GetIO();
  IM_ASSERT(io.ConfigFlags & ImGuiConfigFlags_DockingEnable);

  ImGuiViewport *viewport = ImGui::GetMainViewport();

  if (!viewport) {
    Logger::ui->error("No viewport was obtained.");
    return;
  }

  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_MenuBar;

  static ImGuiDockNodeFlags dockspace_flags =
      ImGuiDockNodeFlags_PassthruCentralNode;

  ImGui::Begin("RootDoctSpaceWindow", nullptr, window_flags);

  static ImGuiID dockspace_id = ImGui::GetID(rootDockSpace);
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

  if (willResetLayout) {
    willResetLayout = false;
    resetLayout();
    resetBitFieldsValues();
  }

  ImGui::End();
}

void UI::createMainMenuBar() {
  // TODO: Separate source file for all menu functions; So when shortcut is
  // invoked, it'll directly call the function
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Project")) {
      if (ImGui::MenuItem("Open Project...", "Ctrl+O", false, false)) {
        // TODO: read project's metadata
        Logger::ui->info("Opening a project...");
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);

        if (result == NFD_OKAY) {
          std::string path(outPath);
          ::free(outPath);
          Logger::ui->info("File selected: {}", path);
        } else if (result == NFD_CANCEL) {
          Logger::ui->warn("File selection cancelled.");
        } else {
          Logger::ui->warn("NFD Error: {}", NFD_GetError());
        }
      }
      if (ImGui::MenuItem("Save...", "Ctrl+S", false, false)) {
        Logger::ui->info("Saving project...");
        // TODO: save project's metadata
        bool success = false;
        if (success) {
          Logger::ui->info("Successfully saved project.");
        } else {
          Logger::ui->warn("Failed to save project.");
        }
      }
      if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S", false, false)) {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {
      }
      if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Reset Layout", "Ctrl+Shift+R", false, false)) {
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void UI::resetLayout() {
  Logger::ui->info("Resetting layout...");
  ImGuiID dockspace_id = ImGui::GetID(rootDockSpace);

  ImGui::DockBuilderRemoveNode(dockspace_id); // clear any existing layout
  ImGui::DockBuilderAddNode(
      dockspace_id, ImGuiDockNodeFlags_DockSpace); // creates a new dock node
  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

  ImGuiID dock_main_id = dockspace_id;
  ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(
      dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);
  ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(
      dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);

  ImGui::DockBuilderDockWindow("Render Buffer", dock_main_id);
  ImGui::DockBuilderDockWindow("Left Panel", dock_left_id);
  ImGui::DockBuilderDockWindow("Right Panel", dock_right_id);

  ImGui::DockBuilderFinish(dockspace_id);
}

void UI::resetBitFieldsValues() {
  Logger::ui->info("Resetting UI visibility bit fields values...");
  uiVisibility.left_panel = 1;
  uiVisibility.render_buffer = 1;
  uiVisibility.right_panel = 1;
}

void UI::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  createRootDockSpace();
  createMainMenuBar();

  ImGuiIO &io = ImGui::GetIO();

  // ImGui::ShowDemoWindow();
  renderImGuiWindows();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
    SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
  }
}

void UI::renderImGuiWindows() {
  static bool open = false;

  if (uiVisibility.left_panel) {
    open = uiVisibility.left_panel;
    ImGui::Begin("Left Panel", &open);
    ImGui::Text("Reset Layout");
    static int b_ResetLayout = 0;
    if (ImGui::Button("Reset"))
      b_ResetLayout++;
    if (b_ResetLayout & 1) {
      willResetLayout = true;
      b_ResetLayout++;
    }
    ImGui::End();
    uiVisibility.left_panel = open;
  }

  if (uiVisibility.render_buffer) {
    open = uiVisibility.render_buffer;
    ImGui::Begin("Render Buffer", &open);
    ImGui::End();
    uiVisibility.render_buffer = open;
  }

  if (uiVisibility.right_panel) {
    open = uiVisibility.right_panel;
    ImGui::Begin("Right Panel", &open);
    ImGui::Text("Meow!");
    ImGui::End();
    uiVisibility.right_panel = open;
  }
}

void UI::free() {
  Logger::ui->info("Destroying ImGUI resources...");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  Logger::ui->info("Successfully destroyed ImGUI resources.");
}
