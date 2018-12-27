#include <utility>

//
// Created by sandy on 25/12/18.
//

#include <Input.h>
#include "private/Input/InputManager.h"

namespace sce::input {

private_::InputManager gInputManager;

void Initialize() {
  gInputManager.Initialize(false);
}

void Terminate() {
  gInputManager.Terminate(false);
}

void RunFrame(double dt) {
  gInputManager.RunFrame(dt);
}

void SetKeyboardCallback(CallbackCategory category,
                         std::function<void(int32_t)> callback) {
  gInputManager.SetKeyboardCallback(category, std::move(callback));
}

void SetMouseButtonCallback(CallbackCategory category,
                            std::function<void(int32_t, int32_t, int32_t)> callback) {
  gInputManager.SetMouseButtonCallback(category, std::move(callback));
}

void SetMouseMotionCallback(std::function<void(int32_t, int32_t)> callback)
{
  gInputManager.SetMouseMotionCallback(std::move(callback));
}

}  // namespace sce::input
