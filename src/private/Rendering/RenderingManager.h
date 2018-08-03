//
// Created by sandy on 08/06/18.
//

#ifndef SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_H
#define SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_H

#include <SDL2/SDL_system.h>
#include <SDL2/SDL_video.h>
#include <Scene.h>
#include <memory>
#include <vec4.hpp>
#include "../Manager.h"

namespace sce::rendering::private_ {
class RenderingManager : public sce::private_::Manager {
  const uint32_t windowFlags_;
  bool commandQueueDirty_;  ///< Require a rebuild of command buffer
 protected:
  SDL_Window * window_;
  glm::vec4 clearColor_;

  virtual void InitializeInternal() = 0;
  virtual void TerminateInternal() = 0;
  virtual void GenerateCommands() = 0;
  virtual void Submit() = 0;
  virtual void Present() const = 0;
  void SetClearValue(glm::vec4 clearColor);

 public:
  explicit RenderingManager(uint32_t windowFlags);

  void Initialize() final;
  void Terminate() final;

  void RunFrame(double dt, std::shared_ptr<scene::Scene> currentScene);
};
}  // namespace sce::rendering::private_

#endif  // SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_H
