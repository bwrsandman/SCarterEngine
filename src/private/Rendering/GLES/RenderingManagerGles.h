//
// Created by sandy on 10/06/18.
//

#ifndef SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_GLES_H
#define SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_GLES_H

#include "../RenderingManager.h"
#include "CommandQueueGles.h"

namespace sce::rendering::private_ {
class RenderingManagerGles : public RenderingManager {
  SDL_GLContext context_;
  CommandQueueGles commandQueue_;

  void InitializeInternal() override;
  void TerminateInternal() override;
  void GenerateCommands() override;
  void Submit() override;
  void Present() const override;

 public:
  RenderingManagerGles();
};
}  // namespace sce::rendering::private_

#endif  // SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_GLES_H
