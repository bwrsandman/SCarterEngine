//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_GAME_PRIVATE_GAMEMANAGER_H
#define SCARTERENGINE_GAME_PRIVATE_GAMEMANAGER_H

#include <Scene.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "../Manager.h"

namespace sce::game::private_ {

class GameManager : public sce::private_::Manager {
  std::unordered_map<std::string, std::shared_ptr<scene::Scene>> scenes_;

 public:
  void Initialize() override;
  void Terminate() override;
  std::shared_ptr<scene::Scene> AddScene(const std::string & name);
  void RemoveScene(const std::string & name);
};

}  // namespace sce::game::private_

#endif  // SCARTERENGINE_GAME_PRIVATE_GAMEMANAGER_H
