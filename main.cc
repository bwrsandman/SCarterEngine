#include <cstdlib>

#include <Logging.h>

int main() {
  sce::logging::Initialize();

  sce::logging::Terminate();

  return 0;
}
