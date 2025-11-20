#define SDL_MAIN_HANDLED
#include "Game.h"

int main(int argc, char *argv[]) {
  Game *game = Game::getInstance();
  game->run();
  return 0;
}
