#include <cstdlib>

#include <Logging.h>
#include <Scripting.h>

int main() {
  sce::logging::Initialize();
  sce::scripting::Initialize();

  sce::scripting::RunFile("main.lua");

  sce::scripting::Terminate();
  sce::logging::Terminate();

  return 0;
}
