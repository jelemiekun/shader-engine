#define SDL_MAIN_HANDLED
#include "Engine.h"
#include "Logger.h"

int main(int argc, char *argv[]) {
  Logger::init();
  Engine::getInstance()->run();
  return 0;
}
