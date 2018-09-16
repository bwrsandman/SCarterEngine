//
// Created by sandy on 08/06/18.
//

#ifndef SCARTERENGINE_RENDERING_H
#define SCARTERENGINE_RENDERING_H

#include "enum.h"

#include <Scene.h>
#include <memory>

namespace sce::rendering {

BETTER_ENUM(RenderingApi, uint8_t, Default, Vulkan, OpenGLES);

void Initialize(RenderingApi api = RenderingApi::Default, bool reload = false);
void Terminate(bool reload = false);

void RunFrame(double dt, std::shared_ptr<scene::Scene> currentScene);

}  // namespace sce::rendering

#endif  // SCARTERENGINE_RENDERING_H
