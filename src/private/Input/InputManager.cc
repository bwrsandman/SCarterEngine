//
// Created by sandy on 25/12/18.
//

#include "InputManager.h"
#include <Engine.h>
#include <Logging.h>
#include <SDL_events.h>

void sce::input::private_::InputManager::Initialize(bool reload) {
  keyboardCallbacks.fill(std::function<void(int32_t)>());
  mouseKeyCallbacks.fill(std::function<void(int32_t, int32_t, int32_t)>());
  mouseMotionCallback = std::function<void(int32_t, int32_t)>();
}

void sce::input::private_::InputManager::Terminate(bool reload) {}

void sce::input::private_::InputManager::RunFrame(double dt) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    decltype(keyboardCallbacks)::value_type keyCallback;
    decltype(mouseKeyCallbacks)::value_type mouseKeyCallback;
    switch (e.type) {
      case SDL_QUIT:
        LOG(logging::Level::Debug, "Quit event detected");
        engine::ScheduleQuit();
        break;
      case SDL_KEYDOWN:
        keyCallback = keyboardCallbacks[CallbackCategory::KeyDown - CallbackCategory::KeyDown];
        break;
      case SDL_KEYUP:
        keyCallback = keyboardCallbacks[CallbackCategory::KeyUp - CallbackCategory::KeyDown];
        break;
      case SDL_MOUSEMOTION:
        if (mouseMotionCallback)
          mouseMotionCallback(e.motion.x, e.motion.y);
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseKeyCallback = mouseKeyCallbacks[CallbackCategory::MouseButtonDown - CallbackCategory::MouseButtonDown];
        break;
      case SDL_MOUSEBUTTONUP:
        mouseKeyCallback = mouseKeyCallbacks[CallbackCategory::MouseButtonUp - CallbackCategory::MouseButtonDown];
        break;
    }
    if (keyCallback) {
      keyCallback(e.key.keysym.sym);
    } else if (mouseKeyCallback) {
      mouseKeyCallback(e.button.button, e.button.x, e.button.y);
    }
  }
}

void sce::input::private_::InputManager::SetKeyboardCallback(
    sce::input::CallbackCategory category,
    std::function<void(int32_t)> callback) {
  keyboardCallbacks[category - sce::input::CallbackCategory::KeyDown] = std::move(callback);
}

void sce::input::private_::InputManager::SetMouseButtonCallback(
    sce::input::CallbackCategory category,
    std::function<void(int32_t, int32_t, int32_t)> callback) {
  mouseKeyCallbacks[category - sce::input::CallbackCategory::MouseButtonDown] = std::move(callback);
}

void sce::input::private_::InputManager::SetMouseMotionCallback(
    std::function<void(int32_t, int32_t)> callback) {
  mouseMotionCallback = std::move(callback);
}
