//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_SCENE_H
#define SCARTERENGINE_SCENE_H

#include <memory>
#include <string>
#include "Camera.h"

namespace sce::scene {
class Scene {
 public:
  virtual std::string Name() const = 0;
  virtual std::shared_ptr<camera::Camera> AddCamera(std::string name) = 0;
  virtual void RemoveCamera(std::string name) = 0;
};
}  // namespace sce::scene

#endif  // SCARTERENGINE_SCENE_H
