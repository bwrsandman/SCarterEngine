//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_SCENE_H
#define SCARTERENGINE_SCENE_H

#include <string>

namespace sce::scene {
class Scene {
 public:
  virtual std::string Name() const = 0;
};
}  // namespace sce::scene

#endif  // SCARTERENGINE_SCENE_H