//
// Created by sandy on 10/06/18.
//

#include <Engine.h>
#include <cstdlib>

#include "private/Engine/EngineManager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

namespace sce::engine {
static private_::EngineManager gEngineManager;

#ifdef __EMSCRIPTEN__
void EnscriptenMainLoop() {
  auto shouldQuit = gEngineManager.RunFrame();
  if (shouldQuit) {
    gEngineManager.Terminate();
    emscripten_cancel_main_loop();
  }
}
#endif

int App() {
  gEngineManager.Initialize();
#ifdef __EMSCRIPTEN__
  int fps = 0;
  emscripten_set_main_loop(EnscriptenMainLoop, fps, true);
#else
  while (!gEngineManager.RunFrame()) {
  }
  gEngineManager.Terminate();
#endif
  return EXIT_SUCCESS;
}

void ScheduleQuit() {
  gEngineManager.ScheduleQuit();
}

}  // namespace sce::engine
