//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_PRIVATE_SCENEIMPL_H
#define SCARTERENGINE_PRIVATE_SCENEIMPL_H

#include <Scene.h>
#include <string>
#include <unordered_map>

namespace sce::scene::private_ {

class SceneImpl : public Scene {
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<camera::Camera>> cameras_;

 public:
  explicit SceneImpl(std::string name);
  std::string Name() const override;
  std::unordered_map<std::string, std::shared_ptr<camera::Camera>> GetCameras()
      const override;
  std::shared_ptr<camera::Camera> AddCamera(std::string name) override;
  void RemoveCamera(std::string name) override;
};

}  // namespace sce::scene::private_

#endif  // SCARTERENGINE_PRIVATE_SCENEIMPL_H
