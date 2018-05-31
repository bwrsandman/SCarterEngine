//
// Created by sandy on 31/05/18.
//

#include "GameManager.h"
#include <Debug.h>
#include <Logging.h>
#include <Scripting.h>
#include "../Scene/SceneImpl.h"

namespace sce::game::private_ {

void GameManager::Initialize() {
  DEBUG_RUNTIME_ASSERT_FALSE(this->isInitialized);
  DEBUG_RUNTIME_ASSERT_TRUE(scenes_.empty());
  isInitialized = true;
}

void GameManager::Terminate() {
  DEBUG_RUNTIME_ASSERT_TRUE(this->isInitialized);
  scenes_.clear();
  isInitialized = false;
}

std::shared_ptr<scene::Scene> GameManager::AddScene(const std::string & name) {
  LOG(logging::Level::Debug, "Adding Scene: \"" + name + "\"");
  auto insertion =
      scenes_.emplace(name, std::make_shared<scene::private_::SceneImpl>(name));
  DEBUG_RUNTIME_ASSERT_TRUE(insertion.second);
  return insertion.first->second;
}

void GameManager::RemoveScene(const std::string & name) {
  LOG(logging::Level::Debug, "Removing Scene: \"" + name + "\"");
  auto extraction = scenes_.find(name);
  DEBUG_RUNTIME_ASSERT_TRUE(extraction != scenes_.end());
  if (extraction != scenes_.end()) {
    scenes_.erase(extraction);
  }
}

}  // namespace sce::game::private_
