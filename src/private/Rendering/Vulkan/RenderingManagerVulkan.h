//
// Created by sandy on 08/06/18.
//

#ifndef SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_VULKAN_H
#define SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_VULKAN_H

#include <set>
#include <vulkan/vulkan.hpp>
#include "../RenderingManager.h"

namespace sce::rendering::private_ {

struct SwapchainImageResources {
  vk::Image image;
  vk::CommandBuffer cmd;
  vk::ImageView view;
  vk::Framebuffer framebuffer;
  vk::Fence fence;
};

const std::set<std::string> required_extensions({
    VK_KHR_SURFACE_EXTENSION_NAME,
});

const std::set<std::string> required_layers({});

class RenderingManagerVulkan : public RenderingManager {
  const bool validation_;

  vk::UniqueInstance instance_;
  vk::UniqueDevice device_;
  vk::UniqueSwapchainKHR swapchain_;
  std::vector<SwapchainImageResources> swapchainImages_;
  vk::UniqueSemaphore acquireCompleteSemaphore_;
  vk::UniqueSemaphore renderCompleteSemaphore_;
  vk::Fence acquireFence_;
  vk::SurfaceKHR surface_;
  vk::Queue deviceGraphicsQueue_;
  vk::Queue devicePresentQueue_;
  vk::RenderPass renderPass_;
  vk::UniqueCommandPool commandPool_;
  vk::UniqueCommandBuffer commandBuffer_;

  vk::Extent2D swapchainExtent_;
  vk::Format colorFormat_;
  vk::ColorSpaceKHR colorSpace_;

  uint32_t currentPresentImageIndex;

  void InitializeInternal() override;
  void TerminateInternal() override;
  void GenerateCommands() override;
  void Submit() override;
  void Present() const override;

  std::set<std::string> GetWindowingExtensions() const;
  std::set<std::string> GetExtensions() const;
  std::set<std::string> GetLayers() const;

 public:
  RenderingManagerVulkan();
};
}  // namespace sce::rendering::private_

#endif  // SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_VULKAN_H
