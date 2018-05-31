//
// Created by sandy on 31/05/18.
//

#include "SceneImpl.h"

namespace sce::scene::private_ {
SceneImpl::SceneImpl(std::string name) : name_(name) {}
std::string SceneImpl::Name() const {
  return name_;
}
}  // namespace sce::scene::private_
