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

void SetCallback(CallbackCategory category,
                 std::function<void(int32_t)> callback) {
  gInputManager.SetCallback(category, callback);
}

}  // namespace sce::input
