#define SDL_MAIN_HANDLED
#include "Engine.h"

int main(int argc, char *argv[]) {
  Engine::getInstance()->run();
  return 0;
}
