//
// Created by sandy on 25/12/18.
//

#ifndef SCARTERENGINE_INPUT_INPUTMANAGER_H
#define SCARTERENGINE_INPUT_INPUTMANAGER_H

#include "../Manager.h"

namespace sce::input::private_ {
class InputManager : sce::private_::Manager {
 public:
  void Initialize(bool reload) override;
  void Terminate(bool reload) override;
  void RunFrame(double dt);
};
}  // namespace sce::input::private_

#endif  // SCARTERENGINE_INPUT_INPUTMANAGER_H
