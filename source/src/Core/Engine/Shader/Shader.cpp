#include "Shader.h"
#include "Game.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <sstream>

Shader::Shader() : usable(false) {}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::init(const char *sourcePath) {
  std::string vertexShaderSource =
      parseShaderSource(sourcePath, Shader_Type::Vertex);
  std::string fragmentShaderSource =
      parseShaderSource(sourcePath, Shader_Type::Fragment);

  GLuint vertexShader =
      compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
  GLuint fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

  if (vertexShader == 0 || fragmentShader == 0) {
    spdlog::error("Failed to create shader.");
    return;
  } else {
    spdlog::info("Vertex and Fragment shader created successfully.");
  }

  createProgram(vertexShader, fragmentShader);
}

std::string Shader::parseShaderSource(const char *sourcePath,
                                      Shader_Type type) {
  std::ifstream stream(sourcePath);
  if (!stream) {
    spdlog::error("Failed to open shader file: {}", sourcePath);
    return "";
  }

  Shader_Type currentType = Shader_Type::None;
  std::stringstream vertexCode, fragmentCode;
  std::string line;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        currentType = Shader_Type::Vertex;
      } else if (line.find("fragment") != std::string::npos) {
        currentType = Shader_Type::Fragment;
      }
    } else {
      if (currentType == Shader_Type::Vertex) {
        vertexCode << line << '\n';
      } else if (currentType == Shader_Type::Fragment) {
        fragmentCode << line << '\n';
      }
    }
  }

  return (type == Shader_Type::Vertex) ? vertexCode.str() : fragmentCode.str();
}

GLuint Shader::compileShader(GLuint shader_Type, const char *source) {
  GLuint shader = glCreateShader(shader_Type);

  if (shader == 0) {
    spdlog::error("Failed to create {} Shader.",
                  (shader_Type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment");
    return 0;
  }

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int shaderSuccess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
  std::string shaderTypeString =
      (shader_Type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
  if (shaderSuccess) {
    spdlog::info("{} Shader compile success.", shaderTypeString);
    return shader;
  } else {
    char log[512];
    glGetShaderInfoLog(shader, 512, NULL, log);
    spdlog::warn("{} Shader failed to compile: {}", shaderTypeString, log);
    return 0;
  }
}

void Shader::createProgram(GLuint &vertexShader, GLuint &fragmentShader) {
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);
  glValidateProgram(ID);

  validateProgram();

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::validateProgram() {
  int shaderProgramSuccess;
  glGetProgramiv(ID, GL_LINK_STATUS, &shaderProgramSuccess);
  if (shaderProgramSuccess) {
    spdlog::info("Shader program linking success.");
    usable = true;
  } else {
    char log[512];
    glGetProgramInfoLog(ID, 512, NULL, log);
    spdlog::warn("Shader program failed to link: {}", log);
  }
}

int Shader::getUniformLocation(const std::string &name) {
  if (uniformLocationCache.find(name) != uniformLocationCache.end())
    return uniformLocationCache[name];

  int location = glGetUniformLocation(ID, name.c_str());

  if (location == -1)
    spdlog::warn("Warning: Uniform {} doesn't exist!", name);

  uniformLocationCache[name] = location;
  return location;
}

void Shader::bind() const {
  if (usable)
    glUseProgram(ID);
  else
    spdlog::warn("Unusable shader program.");

  // TODO: remove after use
  if (!usable) {
    Game *game = Game::getInstance();
    game->m_Running = false;
  }
}

void Shader::unbind() const { glUseProgram(0); }

void Shader::setBool(const std::string &name, bool value) {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setInt(const std::string &name, int value) {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) {
  glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) {
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
  glUniform3f(getUniformLocation(name), value.r, value.g, value.b);
}

void Shader::free() {
  if (usable) {
    spdlog::info("Cleaning up shader program (ID: {})", ID);
    glUseProgram(0);
    glDeleteProgram(ID);
    ID = 0;
    usable = false;
    uniformLocationCache.clear();
  } else if (ID != 0) {
    spdlog::info("Cleaning up invalid or incomplete shader program (ID: {})",
                 ID);
    glDeleteProgram(ID);
    ID = 0;
    uniformLocationCache.clear();
  } else {
    spdlog::debug("Shader::clean() called but no program to delete.");
  }
}
