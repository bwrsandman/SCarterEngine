//
// Created by sandy on 07/06/18.
//

#ifndef SCARTERENGINE_CAMERA_H
#define SCARTERENGINE_CAMERA_H

#include <string>
#include <vec4.hpp>

namespace sce::camera {
class Camera {
 public:
  virtual std::string Name() const = 0;
  virtual glm::vec4 GetClearValue() const = 0;
  virtual void SetClearValue(glm::vec4 clearColor) = 0;
};
}  // namespace sce::camera

#endif  // SCARTERENGINE_CAMERA_H
