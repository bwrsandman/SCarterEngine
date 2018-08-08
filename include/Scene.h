//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_SCENE_H
#define SCARTERENGINE_SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "Camera.h"
#include "Mesh.h"

namespace sce::scene {
class Scene {
 public:
  virtual std::string Name() const = 0;
  virtual std::unordered_map<std::string, std::shared_ptr<camera::Camera>>
  GetCameras() const = 0;
  virtual std::shared_ptr<camera::Camera> AddCamera(std::string name) = 0;
  virtual void RemoveCamera(std::string name) = 0;

  virtual std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>>
  GetMeshes() const = 0;
  virtual std::shared_ptr<mesh::Mesh> AddMesh(
      std::string name, std::vector<mesh::Index> indices,
      std::vector<mesh::Vertex> vertices) = 0;
  virtual void RemoveMesh(std::string name) = 0;
};
}  // namespace sce::scene

#endif  // SCARTERENGINE_SCENE_H
