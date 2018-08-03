//
// Created by sandy on 31/05/18.
//

#include "SceneImpl.h"
#include <Debug.h>
#include <Logging.h>
#include "../Camera/CameraImpl.h"

namespace sce::scene::private_ {
SceneImpl::SceneImpl(std::string name) : name_(name) {}
std::string SceneImpl::Name() const {
  return name_;
}

std::shared_ptr<camera::Camera> SceneImpl::AddCamera(std::string name) {
  LOG(logging::Level::Debug, "Adding Camera: \"" + name + "\"");
  auto insertion = cameras_.emplace(
      name, std::make_shared<camera::private_::CameraImpl>(name));
  DEBUG_RUNTIME_ASSERT_TRUE(insertion.second);
  return insertion.first->second;
}

void SceneImpl::RemoveCamera(std::string name) {
  LOG(logging::Level::Debug, "Removing Camera: \"" + name + "\"");
  auto extraction = cameras_.find(name);
  DEBUG_RUNTIME_ASSERT_TRUE(extraction != cameras_.end());
  if (extraction != cameras_.end()) {
    cameras_.erase(extraction);
  }
}
}  // namespace sce::scene::private_
