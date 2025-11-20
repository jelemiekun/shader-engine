#pragma once
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Logger {
extern std::shared_ptr<spdlog::logger> camera;
extern std::shared_ptr<spdlog::logger> elementBuffer;
extern std::shared_ptr<spdlog::logger> engine;
extern std::shared_ptr<spdlog::logger> logger;
extern std::shared_ptr<spdlog::logger> mesh;
extern std::shared_ptr<spdlog::logger> model;
extern std::shared_ptr<spdlog::logger> physics;
extern std::shared_ptr<spdlog::logger> rigidBody;
extern std::shared_ptr<spdlog::logger> shader;
extern std::shared_ptr<spdlog::logger> texture2D;
extern std::shared_ptr<spdlog::logger> ui;
extern std::shared_ptr<spdlog::logger> vertexArray;
extern std::shared_ptr<spdlog::logger> vertexBuffer;

void init();
void free();
}; // namespace Logger
