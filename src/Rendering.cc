//
// Created by sandy on 08/06/18.
//

#include <Debug.h>
#include <Logging.h>
#include <Rendering.h>
#include <memory>
#include "private/Rendering/GLES/RenderingManagerGles.h"
#ifdef SCE_SUPPORTS_VULKAN
#include "private/Rendering/Vulkan/RenderingManagerVulkan.h"
#endif

namespace sce::rendering {

static std::unique_ptr<private_::RenderingManager> gRenderingManager;

void Initialize(RenderingApi api) {
  if (gRenderingManager)
    gRenderingManager->Terminate();

  switch (api) {
    case RenderingApi::Default:
#ifdef SCE_SUPPORTS_VULKAN
    case RenderingApi::Vulkan:
      gRenderingManager = std::make_unique<private_::RenderingManagerVulkan>();
      break;
#endif
    case RenderingApi::OpenGLES:
      gRenderingManager = std::make_unique<private_::RenderingManagerGles>();
      break;
    default:
      LOG(logging::Level::Fatal, "Unsupported Rendering API");
      DEBUG_RUNTIME_ASSERT_TRUE(false);
      break;
  }
  gRenderingManager->Initialize();
}

void Terminate() {
  gRenderingManager->Terminate();
}

void RunFrame(double dt, std::shared_ptr<scene::Scene> currentScene) {
  if (!currentScene) {
    LOG(logging::Level::Fatal, "No scene set to render");
  }
  DEBUG_RUNTIME_ASSERT_NOT_NULL(currentScene.get());
  gRenderingManager->RunFrame(dt, std::move(currentScene));
}

}  // namespace sce::rendering
