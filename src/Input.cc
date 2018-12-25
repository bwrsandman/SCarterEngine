//
// Created by sandy on 25/12/18.
//

#include <Input.h>
#include "private/Input/InputManager.h"

namespace sce::input {

private_::InputManager gInputManager;

void Initialize() {
  gInputManager.Initialize();
}

void Terminate() {
  gInputManager.Terminate();
}

void RunFrame(double dt) {
  gInputManager.RunFrame(dt);
}

}  // namespace sce::input
