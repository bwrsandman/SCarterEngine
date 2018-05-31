//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_GAME_H
#define SCARTERENGINE_GAME_H

#include <Scene.h>
#include <memory>
#include <string>

namespace sce::game {

void Initialize();
void Terminate();
std::shared_ptr<scene::Scene> AddScene(const std::string & name);
void RemoveScene(const std::string & name);

}  // namespace sce::game

#endif  // SCARTERENGINE_GAME_H
