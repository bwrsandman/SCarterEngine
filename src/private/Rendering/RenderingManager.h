//
// Created by sandy on 08/06/18.
//

#ifndef SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_H
#define SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_H

#include <memory>

#include <SDL2/SDL_system.h>
#include <SDL2/SDL_video.h>
#include <vec4.hpp>

#include <Mesh.h>
#include <Scene.h>

#include "../Manager.h"

namespace sce::rendering::private_ {
struct MergedMeshIndexBufferLocation {
  const char * name;
  uint32_t count;
  uint32_t offset;
};
void MergeMeshes(
    std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>> & meshes,
    std::vector<mesh::Index> & indices, std::vector<mesh::Vertex> & vertices,
    std::vector<MergedMeshIndexBufferLocation> & indexBufferLocations);

class RenderingManager : public sce::private_::Manager {
  const uint32_t windowFlags_;
  bool commandQueueDirty_;  ///< Require a rebuild of command buffer
 protected:
  SDL_Window * window_;
  glm::vec4 clearColor_;
  std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>> meshes_;

  virtual void InitializeInternal() = 0;
  virtual void TerminateInternal() = 0;
  virtual void GenerateCommands() = 0;
  virtual void Submit() = 0;
  virtual void Present() const = 0;
  void SetClearValue(glm::vec4 clearColor);
  void SetMeshes(
      std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>> meshes);

 public:
  explicit RenderingManager(uint32_t windowFlags);

  void Initialize() final;
  void Terminate() final;

  void RunFrame(double dt, std::shared_ptr<scene::Scene> currentScene);
};
}  // namespace sce::rendering::private_

#endif  // SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_H
