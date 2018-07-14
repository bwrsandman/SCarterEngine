//
// Created by sandy on 10/06/18.
//

#include "EngineManager.h"

#include <cstdlib>
#include <iostream>

#include <Game.h>
#include <Input.h>
#include <Logging.h>
#include <Rendering.h>
#include <Scripting.h>

namespace sce::engine::private_ {

void EngineManager::Initialize() {
  shouldQuit_ = false;
  sce::logging::Initialize();
  sce::game::Initialize();
  sce::rendering::Initialize();
  sce::input::Initialize();
  sce::scripting::Initialize();
  LOG(logging::Level::Debug, "Engine subsystems initialized");

  sce::scripting::LoadFile("main.lua");

  lastTimeStamp_ = std::chrono::high_resolution_clock::now();
}

void EngineManager::Terminate() {
  LOG(logging::Level::Debug, "Terminating engine subsystems");
  sce::scripting::Terminate();
  sce::input::Terminate();
  sce::rendering::Terminate();
  sce::game::Terminate();
  sce::logging::Terminate();
}

bool EngineManager::RunFrame() {
  auto now = std::chrono::high_resolution_clock::now();
  auto dt =
      std::chrono::duration<double, std::milli>(now - lastTimeStamp_).count();
  lastTimeStamp_ = now;
  if (!shouldQuit_)
    sce::scripting::RunFrame(dt);
  if (!shouldQuit_) {
    sce::input::RunFrame(dt);
  }
  if (!shouldQuit_) {
    sce::rendering::RunFrame(dt, sce::game::GetCurrentScene());
  }
  return shouldQuit_;
}

void EngineManager::ScheduleQuit() {
  shouldQuit_ = true;
}

}  // namespace sce::engine::private_
