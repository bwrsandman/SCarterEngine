//
// Created by sandy on 10/06/18.
//

#ifndef SCARTERENGINE_PRIVATE_ENGINE_ENGINEMANAGER_H
#define SCARTERENGINE_PRIVATE_ENGINE_ENGINEMANAGER_H

#include <chrono>
#include "../Manager.h"

namespace sce::engine::private_ {
class EngineManager : sce::private_::Manager {
  bool shouldQuit_;
  std::chrono::high_resolution_clock::time_point lastTimeStamp_;

 public:
  void Initialize(bool reload) override;
  void Terminate(bool reload) override;
  void Reload();

  bool RunFrame();
  void ScheduleQuit();
};
}  // namespace sce::engine::private_

#endif  // SCARTERENGINE_PRIVATE_ENGINE_ENGINEMANAGER_H
