#include "Camera.h"
// #include "imgui/imgui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>

namespace ProgramValues {
namespace KeyEvents {
bool isLockedIn = true;
bool isLockedInPressed = false;
bool moveForwardPressed = false;
bool moveBackwardPressed = false;
bool moveLeftPressed = false;
bool moveRightPressed = false;
bool moveUpPressed = false;
bool moveDownPressed = false;
bool sprinting = false;
bool fastZoom = false;
} // namespace KeyEvents
} // namespace ProgramValues

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw,
               float startPitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(0.05f), sensitivity(0.1f) {
  position = startPosition;
  worldUp = startUp;
  yaw = startYaw;
  pitch = startPitch;
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(SDL_Event &event, SDL_Window *window) {
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      if (!ProgramValues::KeyEvents::isLockedInPressed) {
        ProgramValues::KeyEvents::isLockedInPressed = true;
        ProgramValues::KeyEvents::isLockedIn =
            !ProgramValues::KeyEvents::isLockedIn;

        if (!ProgramValues::KeyEvents::isLockedIn) {
          int width, height;
          SDL_GetWindowSize(window, &width, &height);
          SDL_WarpMouseInWindow(window, width / 2, height / 2);
        }
      }
      break;
    case SDLK_w:
      ProgramValues::KeyEvents::moveForwardPressed = true;
      break;
    case SDLK_a:
      ProgramValues::KeyEvents::moveLeftPressed = true;
      break;
    case SDLK_s:
      ProgramValues::KeyEvents::moveBackwardPressed = true;
      break;
    case SDLK_d:
      ProgramValues::KeyEvents::moveRightPressed = true;
      break;
    case SDLK_SPACE:
      ProgramValues::KeyEvents::moveUpPressed = true;
      break;
    case SDLK_LCTRL:
      ProgramValues::KeyEvents::moveDownPressed = true;
      break;
    case SDLK_r:
      ProgramValues::KeyEvents::sprinting = true;
      break;
    case SDLK_LSHIFT:
      ProgramValues::KeyEvents::fastZoom = true;
      break;
    default:
      break;
    }
  } else if (event.type == SDL_KEYUP) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      if (ProgramValues::KeyEvents::isLockedInPressed) {
        ProgramValues::KeyEvents::isLockedInPressed = false;
      }
      break;
    case SDLK_w:
      ProgramValues::KeyEvents::moveForwardPressed = false;
      break;
    case SDLK_a:
      ProgramValues::KeyEvents::moveLeftPressed = false;
      break;
    case SDLK_s:
      ProgramValues::KeyEvents::moveBackwardPressed = false;
      break;
    case SDLK_d:
      ProgramValues::KeyEvents::moveRightPressed = false;
      break;
    case SDLK_SPACE:
      ProgramValues::KeyEvents::moveUpPressed = false;
      break;
    case SDLK_LCTRL:
      ProgramValues::KeyEvents::moveDownPressed = false;
      break;
    case SDLK_r:
      ProgramValues::KeyEvents::sprinting = false;
      break;
    case SDLK_LSHIFT:
      ProgramValues::KeyEvents::fastZoom = false;
      break;
    default:
      break;
    }
  }
}

void Camera::processMouseMotion(SDL_Event &event) {
  if (event.type == SDL_MOUSEMOTION && ProgramValues::KeyEvents::isLockedIn) {
    float xoffset = static_cast<float>(event.motion.xrel);
    float yoffset = static_cast<float>(event.motion.yrel);

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  } else if (event.type == SDL_MOUSEBUTTONDOWN) {
    // ImGuiIO &io = ImGui::GetIO();
    // if (event.button.button == SDL_BUTTON_LEFT &&
    //     !ProgramValues::KeyEvents::isLockedIn && !io.WantCaptureMouse)
    //  ProgramValues::KeyEvents::isLockedIn = true;
  } else if (event.type == SDL_MOUSEWHEEL &&
             ProgramValues::KeyEvents::isLockedIn) {
    float localSensitivity =
        ProgramValues::KeyEvents::fastZoom
            ? event.wheel.y * sensitivity * FOV_SPEED_MULTIPLIER
            : event.wheel.y * sensitivity;
    fov -= localSensitivity;

    if (fov < 1.0f)
      fov = 1.0f;
    if (fov > FOV_MAX)
      fov = FOV_MAX;
  }
}

void Camera::update() {
  SDL_SetRelativeMouseMode(ProgramValues::KeyEvents::isLockedIn ? SDL_TRUE
                                                                : SDL_FALSE);

  if (ProgramValues::KeyEvents::isLockedIn) {
    float modifiedSpeed =
        ProgramValues::KeyEvents::sprinting ? speed * SPRINT_MULTIPLIER : speed;

    if (ProgramValues::KeyEvents::moveForwardPressed)
      position += modifiedSpeed * front;
    if (ProgramValues::KeyEvents::moveLeftPressed)
      position -= glm::normalize(glm::cross(front, up)) * modifiedSpeed;
    if (ProgramValues::KeyEvents::moveBackwardPressed)
      position -= modifiedSpeed * front;
    if (ProgramValues::KeyEvents::moveRightPressed)
      position += glm::normalize(glm::cross(front, up)) * modifiedSpeed;
    if (ProgramValues::KeyEvents::moveUpPressed)
      position += speed * up;
    if (ProgramValues::KeyEvents::moveDownPressed)
      position -= speed * up;

    updateCameraVectors();
  }
}

void Camera::updateCameraVectors() {
  glm::vec3 dir;
  dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  dir.y = sin(glm::radians(pitch));
  dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(dir);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

float Camera::getFOV() const { return fov; }
