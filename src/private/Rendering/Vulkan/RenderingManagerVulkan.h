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
  struct BoundBuffer {
    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory memory;
  };

  const bool validation_;

  vk::UniqueInstance instance_;
  vk::UniqueDevice device_;
  std::unique_ptr<vk::DispatchLoaderDynamic> dynamicDispatchLoader_;
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
  vk::UniquePipelineCache pipelineCache_;
  vk::UniqueCommandBuffer commandBuffer_;
  vk::UniquePipelineLayout pipelineLayout_;
  vk::UniquePipeline pipeline_;

  vk::Extent2D swapchainExtent_;
  vk::Format colorFormat_;
  vk::ColorSpaceKHR colorSpace_;

  vk::PhysicalDeviceMemoryProperties memoryProperties_;

  uint32_t currentPresentImageIndex;

  BoundBuffer sceneIndexBuffer_;
  BoundBuffer sceneVertexBuffer_;

  void InitializeInternal() override;
  void TerminateInternal() override;
  void GenerateCommands() override;
  void Submit() override;
  void Present() const override;

  std::set<std::string> GetWindowingExtensions() const;
  std::set<std::string> GetExtensions() const;
  std::set<std::string> GetLayers() const;

  uint32_t GetMemoryType(uint32_t typeBits, vk::MemoryPropertyFlags properties);
  BoundBuffer CreateBoundBuffer(vk::BufferUsageFlags usage,
                                vk::MemoryPropertyFlags memoryPropertyFlags,
                                vk::DeviceSize dataSize, void * data = nullptr);

 public:
  RenderingManagerVulkan();
};
}  // namespace sce::rendering::private_

#endif  // SCARTERENGINE_PRIVATE_RENDERING_RENDERINGMANAGER_VULKAN_H
