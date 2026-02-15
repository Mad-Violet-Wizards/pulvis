#include "Game.hpp"

#include "Logger.hpp"

#include <iostream>
#include "Filesystem.hpp"

int main()
{
  Game& game = Game::GetInstance();

  PULVIS_INFO_LOG("Current assets path is: {}", pulvis::core::GetAssetsPath().string());
  PULVIS_INFO_LOG("Current workspace path is: {}", pulvis::core::GetWorkspacePath().string());

  game.Initialize();
  game.Run();
  game.Shutdown();

  return 0;
}