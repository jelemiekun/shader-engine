#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// class GameWindow;

class Camera {
public:
  constexpr static float FOV_MAX = 100.0f;
  constexpr static float SPRINT_MULTIPLIER = 3.0f;
  constexpr static float FOV_SPEED_MULTIPLIER = 5.0f;

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;
  float speed;
  float sensitivity;
  float fov = 45.0f;

  Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f),
         glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
         float startYaw = -90.0f, float startPitch = 0.0f);

  glm::mat4 getViewMatrix() const;

  void processKeyboard(SDL_Event &event, SDL_Window *window);

  void processMouseMotion(SDL_Event &event);

  void update();

  float getFOV() const;

private:
  void updateCameraVectors();
};
