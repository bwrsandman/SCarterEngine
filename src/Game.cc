//
// Created by sandy on 31/05/18.
//

#include <Game.h>
#include "private/Game/GameManager.h"

namespace sce::game {

static private_::GameManager gGameManager;

void Initialize() {
  gGameManager.Initialize();
}

void Terminate() {
  gGameManager.Terminate();
}

std::shared_ptr<scene::Scene> AddScene(const std::string & name) {
  return gGameManager.AddScene(name);
}

void RemoveScene(const std::string & name) {
  gGameManager.RemoveScene(name);
}

}  // namespace sce::game
