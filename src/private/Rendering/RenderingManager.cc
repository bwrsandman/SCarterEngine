//
// Created by sandy on 09/06/18.
//

#include "RenderingManager.h"

#include <SDL2/SDL.h>

#include <Debug.h>
#include <Engine.h>
#include <Logging.h>

namespace sce::rendering::private_ {

RenderingManager::RenderingManager(uint32_t windowFlags)
    : window_(nullptr), windowFlags_(windowFlags) {}

void RenderingManager::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LOG(logging::Level::Fatal,
        std::string("Initializing SDL failed: ") + SDL_GetError());
    DEBUG_RUNTIME_ASSERT_TRUE(false);
  }
  window_ = SDL_CreateWindow("SCarter Engine", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, 640, 480, windowFlags_);
  if (window_ == nullptr) {
    LOG(logging::Level::Fatal,
        std::string("Creating SDL window failed: ") + SDL_GetError());
    DEBUG_RUNTIME_ASSERT_NOT_NULL(window_);
  }

  InitializeInternal();
  // Force build of command buffer at runtime, when meshes and cameras should
  // all be loaded and only once
  commandQueueDirty_ = true;
  isInitialized = true;
}

void RenderingManager::Terminate() {
  DEBUG_RUNTIME_ASSERT_TRUE(this->isInitialized);
  TerminateInternal();
  SDL_DestroyWindow(window_);
  SDL_Quit();
  isInitialized = false;
}

void RenderingManager::RunFrame(double dt,
                                std::shared_ptr<scene::Scene> currentScene) {
  if (commandQueueDirty_) {
    GenerateCommands();
    commandQueueDirty_ = false;
  }

  Submit();
  Present();
}

}  // namespace sce::rendering::private_
