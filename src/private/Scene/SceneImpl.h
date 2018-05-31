//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_PRIVATE_SCENEIMPL_H
#define SCARTERENGINE_PRIVATE_SCENEIMPL_H

#include <Scene.h>
#include <string>

namespace sce::scene::private_ {

class SceneImpl : public Scene {
  std::string name_;

 public:
  explicit SceneImpl(std::string name);
  std::string Name() const override;
};

}  // namespace sce::scene::private_

#endif  // SCARTERENGINE_PRIVATE_SCENEIMPL_H
