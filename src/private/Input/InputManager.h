//
// Created by sandy on 25/12/18.
//

#ifndef SCARTERENGINE_INPUT_INPUTMANAGER_H
#define SCARTERENGINE_INPUT_INPUTMANAGER_H

#include <Input.h>
#include <array>
#include "../Manager.h"

namespace sce::input::private_ {
class InputManager : sce::private_::Manager {
  std::array<std::function<void(int32_t)>, 2> keyboardCallbacks{};
  std::array<std::function<void(int32_t, int32_t, int32_t)>, 2> mouseKeyCallbacks{};
  std::function<void(int32_t, int32_t)> mouseMotionCallback{};

 public:
  void Initialize(bool reload) override;
  void Terminate(bool reload) override;
  void RunFrame(double dt);
  void SetKeyboardCallback(CallbackCategory category,
                           std::function<void(int32_t)> callback);
  void SetMouseButtonCallback(CallbackCategory category,
                              std::function<void(int32_t, int32_t, int32_t)> callback);
  void SetMouseMotionCallback(std::function<void(int32_t, int32_t)> callback);
};
}  // namespace sce::input::private_

#endif  // SCARTERENGINE_INPUT_INPUTMANAGER_H
