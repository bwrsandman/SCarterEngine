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
  // Set ClearColor Uniform
  if (currentScene) {
    // For each camera
    // Update uniform data, if for some reason, there is data change,
    // rebalance and rebuild command buffers, but do not issue commands every
    // frame (inefficient)
    for (auto cam : currentScene->GetCameras()) {
      SetClearValue(cam.second->GetClearValue());
      SetMeshes(currentScene->GetMeshes());
    }
  }

  if (commandQueueDirty_) {
    GenerateCommands();
    commandQueueDirty_ = false;
  }

  Submit();
  Present();
}

void RenderingManager::SetClearValue(glm::vec4 clearColor) {
  if (clearColor != clearColor_) {
    clearColor_ = clearColor;
    commandQueueDirty_ = true;
  }
}

void RenderingManager::SetMeshes(
    std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>> meshes) {
  if (meshes != meshes_) {
    meshes_ = meshes;
    commandQueueDirty_ = true;
  }
}

void MergeMeshes(
    std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>> & meshes,
    std::vector<mesh::Index> & indices, std::vector<mesh::Vertex> & vertices,
    std::vector<MergedMeshIndexBufferLocation> & indexBufferLocations) {
  for (const auto & [name, mesh] : meshes) {
    auto newIndices = mesh->GetIndices();
    indexBufferLocations.push_back({name.c_str(),
                                    static_cast<uint32_t>(newIndices.size()),
                                    static_cast<uint32_t>(indices.size())});
    indices.reserve(indices.size() + newIndices.size());
    // Move indices start up at new location with 0 being changed to size of
    // current vertex vector
    auto vertexOffset = static_cast<uint32_t>(vertices.size());
    for (auto & index : newIndices) {
      indices.push_back(index + vertexOffset);
    }
    auto newVertices = mesh->GetVertices();
    vertices.reserve(vertices.size() + newVertices.size());
    std::copy(newVertices.begin(), newVertices.end(),
              std::back_inserter(vertices));
  }
}
}  // namespace sce::rendering::private_
