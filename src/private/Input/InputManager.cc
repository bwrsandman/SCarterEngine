//
// Created by sandy on 25/12/18.
//

#include "InputManager.h"
#include <Engine.h>
#include <Logging.h>
#include <SDL_events.h>

void sce::input::private_::InputManager::Initialize() {}

void sce::input::private_::InputManager::Terminate() {}

void sce::input::private_::InputManager::RunFrame(double dt) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      LOG(logging::Level::Debug, "Quit event detected");
      engine::ScheduleQuit();
    }
  }
}
