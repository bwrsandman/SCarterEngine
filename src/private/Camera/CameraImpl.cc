//
// Created by sandy on 08/06/18.
//

#include "CameraImpl.h"

namespace sce::camera::private_ {
CameraImpl::CameraImpl(std::string name) : name_(name) {}

std::string CameraImpl::Name() const {
  return name_;
}
glm::vec4 CameraImpl::GetClearValue() const {
  return clearValue_;
}

void CameraImpl::SetClearValue(glm::vec4 clearColor) {
  clearValue_ = clearColor;
}
}  // namespace sce::camera::private_
