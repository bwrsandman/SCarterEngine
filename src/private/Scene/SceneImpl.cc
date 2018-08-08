//
// Created by sandy on 31/05/18.
//

#include "SceneImpl.h"
#include <Debug.h>
#include <Logging.h>
#include "../Camera/CameraImpl.h"

namespace sce::scene::private_ {
SceneImpl::SceneImpl(std::string name) : name_(name) {}

std::unordered_map<std::string, std::shared_ptr<camera::Camera>>
SceneImpl::GetCameras() const {
  return cameras_;
}

std::string SceneImpl::Name() const {
  return name_;
}

std::shared_ptr<camera::Camera> SceneImpl::AddCamera(std::string name) {
  LOG(logging::Level::Debug, "Adding Camera: \"" + name + "\"");
  auto insertion = cameras_.emplace(
      name, std::make_shared<camera::private_::CameraImpl>(name));
  DEBUG_RUNTIME_ASSERT_TRUE(insertion.second);
  return insertion.first->second;
}

void SceneImpl::RemoveCamera(std::string name) {
  LOG(logging::Level::Debug, "Removing Camera: \"" + name + "\"");
  auto extraction = cameras_.find(name);
  DEBUG_RUNTIME_ASSERT_TRUE(extraction != cameras_.end());
  if (extraction != cameras_.end()) {
    cameras_.erase(extraction);
  }
}

std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>>
SceneImpl::GetMeshes() const {
  return meshes_;
}

class Triangle : public mesh::Mesh {
  const std::string name_;
  const std::vector<mesh::Index> indices_;
  const std::vector<mesh::Vertex> vertices_;

 public:
  Triangle(std::string name, std::vector<mesh::Index> indices,
           std::vector<mesh::Vertex> vertices)
      : name_(std::move(name)), indices_(indices), vertices_(vertices) {}
  std::string Name() const override {
    return name_;
  }
  const std::vector<mesh::Index> GetIndices() const override {
    return indices_;
  };
  const std::vector<mesh::Vertex> GetVertices() const override {
    return vertices_;
  }
};

std::shared_ptr<mesh::Mesh> SceneImpl::AddMesh(
    std::string name, std::vector<mesh::Index> indices,
    std::vector<mesh::Vertex> vertices) {
  LOG(logging::Level::Debug, "Adding Mesh: \"" + name + "\"");
  auto insertion = meshes_.emplace(
      name, std::make_shared<Triangle>(name, indices, vertices));
  DEBUG_RUNTIME_ASSERT_TRUE(insertion.second);
  return insertion.first->second;
}

void SceneImpl::RemoveMesh(std::string name) {
  LOG(logging::Level::Debug, "Removing Mesh: \"" + name + "\"");
  auto extraction = meshes_.find(name);
  DEBUG_RUNTIME_ASSERT_TRUE(extraction != meshes_.end());
  if (extraction != meshes_.end()) {
    meshes_.erase(extraction);
  }
}
}  // namespace sce::scene::private_
