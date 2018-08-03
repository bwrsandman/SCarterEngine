//
// Created by sandy on 08/06/18.
//

#include "CameraImpl.h"

namespace sce::camera::private_ {
CameraImpl::CameraImpl(std::string name) : name_(name) {}

std::string CameraImpl::Name() const {
  return name_;
}
}  // namespace sce::camera::private_
