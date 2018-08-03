//
// Created by sandy on 08/06/18.
//

#ifndef SCARTERENGINE_PRIVATE_CAMERAIMPL_H
#define SCARTERENGINE_PRIVATE_CAMERAIMPL_H

#include <Camera.h>

namespace sce::camera::private_ {
class CameraImpl : public Camera {
  std::string name_;
  glm::vec4 clearValue_;

 public:
  explicit CameraImpl(std::string name);
  std::string Name() const override;
  glm::vec4 GetClearValue() const override;
  void SetClearValue(glm::vec4 clearValue) override;
};
}  // namespace sce::camera::private_

#endif  // SCARTERENGINE_PRIVATE_CAMERAIMPL_H
