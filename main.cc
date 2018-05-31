#include <cstdlib>

#include <Game.h>
#include <Logging.h>
#include <Scripting.h>

int main() {
  sce::logging::Initialize();
  sce::scripting::Initialize();
  sce::game::Initialize();

  sce::scripting::RunFile("main.lua");

  sce::game::Terminate();
  sce::scripting::Terminate();
  sce::logging::Terminate();

  return 0;
}
