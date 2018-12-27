//
// Created by sandy on 25/12/18.
//

#include "InputManager.h"
#include <Engine.h>
#include <Logging.h>
#include <SDL_events.h>

void sce::input::private_::InputManager::Initialize(bool reload) {
  callbacks.fill(std::function<void(int32_t)>());
}

void sce::input::private_::InputManager::Terminate(bool reload) {}

void sce::input::private_::InputManager::RunFrame(double dt) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    decltype(callbacks)::value_type callback;
    switch (e.type) {
      case SDL_QUIT:
        LOG(logging::Level::Debug, "Quit event detected");
        engine::ScheduleQuit();
        break;
      case SDL_KEYDOWN:
        callback = callbacks[CallbackCategory::KeyDown];
        break;
      case SDL_KEYUP:
        callback = callbacks[CallbackCategory::KeyUp];
        break;
    }
    if (callback) {
      callback(e.key.keysym.sym);
    }
  }
}

void sce::input::private_::InputManager::SetCallback(
    sce::input::CallbackCategory category,
    std::function<void(int32_t)> callback) {
  callbacks[category] = std::move(callback);
}
