#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Logger {
std::shared_ptr<spdlog::logger> camera;
std::shared_ptr<spdlog::logger> elementBuffer;
std::shared_ptr<spdlog::logger> engine;
std::shared_ptr<spdlog::logger> mesh;
std::shared_ptr<spdlog::logger> model;
std::shared_ptr<spdlog::logger> physics;
std::shared_ptr<spdlog::logger> rigidBody;
std::shared_ptr<spdlog::logger> shader;
std::shared_ptr<spdlog::logger> texture2D;
std::shared_ptr<spdlog::logger> ui;
std::shared_ptr<spdlog::logger> vertexArray;
std::shared_ptr<spdlog::logger> vertexBuffer;

void init() {
  camera = spdlog::stdout_color_mt("Camera");
  elementBuffer = spdlog::stdout_color_mt("ElementBuffer");
  engine = spdlog::stdout_color_mt("Engine");
  mesh = spdlog::stdout_color_mt("Mesh");
  model = spdlog::stdout_color_mt("Model");
  physics = spdlog::stdout_color_mt("Physics");
  rigidBody = spdlog::stdout_color_mt("RigidBody");
  shader = spdlog::stdout_color_mt("Shader");
  texture2D = spdlog::stdout_color_mt("Texture2D");
  ui = spdlog::stdout_color_mt("UI");
  vertexArray = spdlog::stdout_color_mt("VertexArray");
  vertexBuffer = spdlog::stdout_color_mt("VertexBuffer");

  spdlog::set_default_logger(engine);

  spdlog::set_level(spdlog::level::debug);
}

void free() { spdlog::shutdown(); }
} // namespace Logger
