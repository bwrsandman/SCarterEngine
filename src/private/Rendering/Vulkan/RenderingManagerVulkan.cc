//
// Created by sandy on 08/06/18.
//

#include "RenderingManagerVulkan.h"

#include <Debug.h>
#include <Engine.h>
#include <Logging.h>
#include <SDL_hints.h>
#include <SDL_vulkan.h>
#include <set>
#include <vulkan/vulkan.hpp>

namespace sce::rendering::private_ {

RenderingManagerVulkan::RenderingManagerVulkan()
    : RenderingManager(SDL_WINDOW_VULKAN),
      validation_(true),
      currentPresentImageIndex(0),
      colorFormat_(vk::Format::eB8G8R8A8Unorm),
      colorSpace_(vk::ColorSpaceKHR::eSrgbNonlinear) {}

std::set<std::string> RenderingManagerVulkan::GetWindowingExtensions() const {
  uint32_t count = 0;
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &count, nullptr)) {
    LOG(logging::Level::Fatal, SDL_GetError());
    engine::ScheduleQuit();
  }

  auto extensions = std::vector<const char *>(count);
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &count, extensions.data())) {
    LOG(logging::Level::Fatal, SDL_GetError());
    engine::ScheduleQuit();
  }
  auto result = std::set<std::string>();
  for (auto & i : extensions) {
    result.emplace(i);
  }
  return result;
}

std::set<std::string> RenderingManagerVulkan::GetExtensions() const {
  auto properties = vk::enumerateInstanceExtensionProperties();
  auto windowing_extensions = GetWindowingExtensions();
  std::set<std::string> extensions(required_extensions);
  if (validation_) {
    // Debug extension
    extensions.emplace(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  for (auto & ext : windowing_extensions) {
    extensions.insert(ext);
  }
  for (auto & req : extensions) {
    bool found = false;
    for (const auto & property : properties) {
      if (req == property.extensionName) {
        found = true;
        break;
      }
    }
    if (!found) {
      LOG(logging::Level::Fatal, "Could not find the " + req + " extension");
      engine::ScheduleQuit();
    }
  }
  return extensions;
}

std::set<std::string> RenderingManagerVulkan::GetLayers() const {
  auto properties = vk::enumerateInstanceLayerProperties();
  std::set<std::string> layers(required_layers);
  if (validation_) {
    // Debug layers
    auto validLayers = vk::enumerateInstanceLayerProperties();
    for (auto & layer : validLayers) {
      if (layer.layerName ==
          std::string("VK_LAYER_LUNARG_standard_validation")) {
        layers.emplace(layer.layerName);
      }
    }
  }
  for (auto & req : layers) {
    bool found = false;
    for (const auto & property : properties) {
      if (req == property.layerName) {
        found = true;
        break;
      }
    }
    if (!found) {
      LOG(logging::Level::Fatal, "Could not find the " + req + " layer");
      engine::ScheduleQuit();
    }
  }
  return required_layers;
}

#ifndef NDEBUG
vk::Bool32 DebugCallback(uint32_t severity,
                         vk::DebugReportObjectTypeEXT /*objType*/,
                         uint64_t /*srcObject*/, size_t /*location*/,
                         int32_t msgCode, const char * pLayerPrefix,
                         const char * pMsg, void * /*pUserData*/) {
  logging::Level level = logging::Level::Info;
  if (severity &
      static_cast<uint32_t>(vk::DebugReportFlagBitsEXT::eInformation)) {
    level = logging::Level::Info;
  } else if (severity &
             static_cast<uint32_t>(vk::DebugReportFlagBitsEXT::eWarning)) {
    level = logging::Level::Warning;
  } else if (severity & static_cast<uint32_t>(
                            vk::DebugReportFlagBitsEXT::ePerformanceWarning)) {
    level = logging::Level::Warning;
  } else if (severity &
             static_cast<uint32_t>(vk::DebugReportFlagBitsEXT::eError)) {
    level = logging::Level::Error;
  } else if (severity &
             static_cast<uint32_t>(vk::DebugReportFlagBitsEXT::eDebug)) {
    level = logging::Level::Debug;
  }
  auto message = std::string(pMsg);
  if (message.back() == '\n') {
    message = message.substr(0, message.size() - 1);
  }
  LOG(level, std::string("[") + pLayerPrefix + "] Code " +
                 std::to_string(msgCode) + " : " + message);

  return VK_FALSE;
}
#endif

void RenderingManagerVulkan::InitializeInternal() {
  auto to_char = [](std::set<std::string> & v) {
    std::vector<const char *> ret(v.size(), nullptr);
    int i = 0;
    for (auto & s : v)
      ret[i++] = s.data();
    return ret;
  };

  SDL_Vulkan_LoadLibrary(nullptr);

  // Create Instance
  auto appInfo = vk::ApplicationInfo();
  appInfo.pEngineName = "SCarterEngine";
  appInfo.engineVersion = 1;
  appInfo.pApplicationName = appInfo.pEngineName;
  appInfo.applicationVersion = appInfo.engineVersion;
  appInfo.apiVersion = VK_API_VERSION_1_1;

  auto extensions_str = GetExtensions();
  auto extensions = to_char(extensions_str);
  auto layers_str = GetLayers();
  auto layers = to_char(layers_str);

  auto info = vk::InstanceCreateInfo();
#ifndef NDEBUG
  auto debugInfo = vk::DebugReportCallbackCreateInfoEXT(
      vk::DebugReportFlagBitsEXT::eDebug |
          vk::DebugReportFlagBitsEXT::eInformation |
          vk::DebugReportFlagBitsEXT::ePerformanceWarning |
          vk::DebugReportFlagBitsEXT::eWarning |
          vk::DebugReportFlagBitsEXT::eError,
      reinterpret_cast<PFN_vkDebugReportCallbackEXT>(DebugCallback));
  info.pNext = &debugInfo;
#endif

  info.setPApplicationInfo(&appInfo);
  info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  info.ppEnabledExtensionNames = extensions.data();
  info.enabledLayerCount = static_cast<uint32_t>(layers.size());
  info.ppEnabledLayerNames = layers.data();
  instance_ = vk::createInstanceUnique(info);

  // Choose physical device
  auto physicalDevices = instance_->enumeratePhysicalDevices();
  DEBUG_RUNTIME_ASSERT_FALSE(physicalDevices.empty());
  auto physicalDevice = physicalDevices[0];

  // Create surface
  SDL_Vulkan_CreateSurface(window_, instance_.get(),
                           reinterpret_cast<VkSurfaceKHR *>(&surface_));
  if (!surface_) {
    LOG(logging::Level::Fatal, SDL_GetError());
    engine::ScheduleQuit();
  }

  // Get device
  auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

  std::vector<vk::Bool32> supportsPresent(queueFamilyProperties.size());
  for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    supportsPresent[i] = physicalDevice.getSurfaceSupportKHR(i, surface_);

  for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    supportsPresent[i] = physicalDevice.getSurfaceSupportKHR(i, surface_);

  // Search for a graphics and a present queue in the array of queue
  // families, try to find one that supports both
  auto graphicsQueueFamilyIndex = std::numeric_limits<uint32_t>::max();
  auto presentQueueFamilyIndex = std::numeric_limits<uint32_t>::max();
  for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
    if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
      if (graphicsQueueFamilyIndex == std::numeric_limits<uint32_t>::max()) {
        graphicsQueueFamilyIndex = i;
      }

      if (supportsPresent[i] == VK_TRUE) {
        graphicsQueueFamilyIndex = i;
        presentQueueFamilyIndex = i;
        break;
      }
    }
  }

  DEBUG_RUNTIME_ASSERT_TRUE(graphicsQueueFamilyIndex <
                            queueFamilyProperties.size());

  DEBUG_RUNTIME_ASSERT_FALSE(
      graphicsQueueFamilyIndex == std::numeric_limits<uint32_t>::max() ||
      presentQueueFamilyIndex == std::numeric_limits<uint32_t>::max());

  auto validDeviceExtensions =
      physicalDevice.enumerateDeviceExtensionProperties();
  auto deviceExtensions = std::vector<const char *>();
  auto deviceLayers = std::vector<const char *>();
  for (auto & extension : validDeviceExtensions) {
    deviceExtensions.emplace_back(extension.extensionName);
  }

  auto queue_priorities = std::vector<float>({0.0f});
  auto deviceQueueCreateInfo = vk::DeviceQueueCreateInfo();
  deviceQueueCreateInfo.queueFamilyIndex =
      static_cast<uint32_t>(graphicsQueueFamilyIndex);
  deviceQueueCreateInfo.queueCount =
      static_cast<uint32_t>(queue_priorities.size());
  deviceQueueCreateInfo.pQueuePriorities = queue_priorities.data();

  auto deviceInfo =
      vk::DeviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo);
  deviceInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions.size());
  deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
  deviceInfo.enabledLayerCount = static_cast<uint32_t>(deviceLayers.size());
  deviceInfo.ppEnabledLayerNames = deviceLayers.data();
  device_ = physicalDevice.createDeviceUnique(deviceInfo);

  deviceGraphicsQueue_ = device_->getQueue(graphicsQueueFamilyIndex, 0);
  devicePresentQueue_ = device_->getQueue(presentQueueFamilyIndex, 0);

  // TODO: get the pipeline cache

  // TODO: get the graphics queue

  // Create Semaphores
  auto semaphoreInfo = vk::SemaphoreCreateInfo();
  acquireCompleteSemaphore_ = device_->createSemaphoreUnique(semaphoreInfo);
  renderCompleteSemaphore_ = device_->createSemaphoreUnique(semaphoreInfo);
  vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);
  acquireFence_ = device_->createFence(fenceInfo);

  // Create Swapchain
  auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface_);
  if (surfaceFormats.size() != 1 ||
      !(surfaceFormats[0].format == vk::Format::eUndefined)) {
    colorFormat_ = surfaceFormats[0].format;
  }
  colorSpace_ = surfaceFormats[0].colorSpace;

  auto surfCaps = physicalDevice.getSurfaceCapabilitiesKHR(surface_);
  auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface_);

  int width, height;
  SDL_Vulkan_GetDrawableSize(window_, &width, &height);
  if (surfCaps.currentExtent.width == 0xFFFFFFFF) {
    swapchainExtent_.width = static_cast<uint32_t>(width);
    swapchainExtent_.height = static_cast<uint32_t>(height);
    if (swapchainExtent_.width < surfCaps.minImageExtent.width) {
      swapchainExtent_.width = surfCaps.minImageExtent.width;
    } else if (swapchainExtent_.width > surfCaps.maxImageExtent.width) {
      swapchainExtent_.width = surfCaps.maxImageExtent.width;
    }

    if (swapchainExtent_.height < surfCaps.minImageExtent.height) {
      swapchainExtent_.height = surfCaps.minImageExtent.height;
    } else if (swapchainExtent_.height > surfCaps.maxImageExtent.height) {
      swapchainExtent_.height = surfCaps.maxImageExtent.height;
    }
  } else {
    // If the surface size is defined, the swap chain size must match
    swapchainExtent_ = surfCaps.currentExtent;
  }
  width = surfCaps.currentExtent.width;
  height = surfCaps.currentExtent.height;

  auto swapchainPresentMode = vk::PresentModeKHR::eFifo;
  for (auto & presentMode : presentModes) {
    if (presentMode == vk::PresentModeKHR::eMailbox) {
      swapchainPresentMode = vk::PresentModeKHR::eMailbox;
      break;
    }
    if (presentMode == vk::PresentModeKHR::eImmediate) {
      swapchainPresentMode = vk::PresentModeKHR::eImmediate;
    }
  }

  uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
  if ((surfCaps.maxImageCount > 0) &&
      (desiredNumberOfSwapchainImages > surfCaps.maxImageCount)) {
    desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
  }

  vk::SurfaceTransformFlagBitsKHR preTransform;
  if (surfCaps.supportedTransforms &
      vk::SurfaceTransformFlagBitsKHR::eIdentity) {
    preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
  } else {
    preTransform = surfCaps.currentTransform;
  }

  auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR();
  swapchainCreateInfo.surface = surface_;
  swapchainCreateInfo.minImageCount = desiredNumberOfSwapchainImages;
  swapchainCreateInfo.imageFormat = colorFormat_;
  swapchainCreateInfo.imageColorSpace = colorSpace_;
  swapchainCreateInfo.imageExtent = swapchainExtent_;
  swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  swapchainCreateInfo.preTransform = preTransform;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
  swapchainCreateInfo.presentMode = swapchainPresentMode;
  swapchainCreateInfo.clipped = VK_TRUE;
  swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  swapchain_ = device_->createSwapchainKHRUnique(swapchainCreateInfo);

  // Get swapchain images
  auto swapchainImagesKHR = device_->getSwapchainImagesKHR(swapchain_.get());
  swapchainImages_.resize(swapchainImagesKHR.size());

  auto swapchainViewInfo = vk::ImageViewCreateInfo();
  swapchainViewInfo.viewType = vk::ImageViewType::e2D;
  swapchainViewInfo.format = colorFormat_;
  swapchainViewInfo.components.r = vk::ComponentSwizzle::eR;
  swapchainViewInfo.components.g = vk::ComponentSwizzle::eG;
  swapchainViewInfo.components.b = vk::ComponentSwizzle::eB;
  swapchainViewInfo.components.a = vk::ComponentSwizzle::eA;
  swapchainViewInfo.subresourceRange.aspectMask =
      vk::ImageAspectFlagBits::eColor;
  swapchainViewInfo.subresourceRange.levelCount = 1;
  swapchainViewInfo.subresourceRange.layerCount = 1;

  for (auto i = 0u; i < swapchainImagesKHR.size(); ++i) {
    swapchainViewInfo.image = swapchainImagesKHR[i];
    swapchainImages_[i].image = swapchainImagesKHR[i];
    swapchainImages_[i].view = device_->createImageView(swapchainViewInfo);
    swapchainImages_[i].fence = device_->createFence(
        vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
  }

  // Create Render Pass
  auto attachments = std::array<vk::AttachmentDescription, 1>();
  auto attachmentReferences = std::array<vk::AttachmentReference, 1>();
  auto subpasses = std::array<vk::SubpassDescription, 1>();
  auto subpassDependencies = std::array<vk::SubpassDependency, 1>();

  attachments[0].format = colorFormat_;
  attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
  attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
  attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  attachments[0].initialLayout = vk::ImageLayout::eUndefined;
  attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

  attachmentReferences[0].layout = vk::ImageLayout::eColorAttachmentOptimal;

  subpasses[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpasses[0].colorAttachmentCount = attachmentReferences.size();
  subpasses[0].pColorAttachments = attachmentReferences.data();

  auto renderPassInfo = vk::RenderPassCreateInfo();
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
  renderPassInfo.pSubpasses = subpasses.data();
  renderPass_ = device_->createRenderPass(renderPassInfo);

  // Create command pool
  auto poolCreateInfo = vk::CommandPoolCreateInfo();
  poolCreateInfo.queueFamilyIndex = deviceQueueCreateInfo.queueFamilyIndex;
  commandPool_ = device_->createCommandPoolUnique(poolCreateInfo);

  // Create Frame Buffers
  auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo(
      commandPool_.get(), vk::CommandBufferLevel::ePrimary,
      static_cast<uint32_t>(swapchainImages_.size()));
  auto swapchain_cmds =
      device_->allocateCommandBuffers(commandBufferAllocateInfo);
  std::array<vk::ImageView, 1> swapbufferAttachments;
  for (uint32_t i = 0; i < swapchainImages_.size(); i++) {
    swapbufferAttachments[0] = swapchainImages_[i].view;
    auto framebufferCreateInfo = vk::FramebufferCreateInfo();
    framebufferCreateInfo.renderPass = renderPass_;
    framebufferCreateInfo.attachmentCount = swapbufferAttachments.size();
    framebufferCreateInfo.pAttachments = swapbufferAttachments.data();
    framebufferCreateInfo.width = static_cast<uint32_t>(width);
    framebufferCreateInfo.height = static_cast<uint32_t>(height);
    framebufferCreateInfo.layers = 1;
    swapchainImages_[i].framebuffer =
        device_->createFramebuffer(framebufferCreateInfo);
    swapchainImages_[i].cmd = swapchain_cmds[i];
  }
}

void RenderingManagerVulkan::GenerateCommands() {
  // Create Command Buffer
  auto allocInfo = vk::CommandBufferAllocateInfo(
      commandPool_.get(), vk::CommandBufferLevel::ePrimary, 1);
  commandBuffer_ =
      std::move(device_->allocateCommandBuffersUnique(allocInfo)[0]);

  vk::ClearValue clearValue =
      vk::ClearColorValue(std::array<float, 4>{0.0f, 1.0f, 0.0f, 0.0f});

  vk::RenderPassBeginInfo renderPassBeginInfo(renderPass_);
  renderPassBeginInfo.renderArea.extent = swapchainExtent_;
  renderPassBeginInfo.clearValueCount = 1;
  renderPassBeginInfo.pClearValues = &clearValue;

  commandBuffer_->begin(vk::CommandBufferBeginInfo{});

  for (auto & swapchainImage : swapchainImages_) {
    // Set target frame buffer
    renderPassBeginInfo.framebuffer = swapchainImage.framebuffer;

    static const auto beginInfo = vk::CommandBufferBeginInfo(
        vk::CommandBufferUsageFlagBits::eSimultaneousUse);
    swapchainImage.cmd.begin(beginInfo);
    swapchainImage.cmd.beginRenderPass(renderPassBeginInfo,
                                       vk::SubpassContents::eInline);
    swapchainImage.cmd.endRenderPass();
    swapchainImage.cmd.end();
  }

  commandBuffer_->end();
}

void RenderingManagerVulkan::TerminateInternal() {
  DEBUG_RUNTIME_ASSERT_TRUE(isInitialized);
  device_->waitIdle();
  device_->waitForFences(acquireFence_, VK_TRUE,
                         std::numeric_limits<uint64_t>::max());
  device_->destroy(acquireFence_);
  commandBuffer_.release();
  commandPool_.release();
  renderCompleteSemaphore_.release();
  acquireCompleteSemaphore_.release();
  swapchainImages_.clear();
  swapchain_.release();
  device_.release();
  instance_.release();
}

void RenderingManagerVulkan::Submit() {
  // Ensure no more than FRAME_LAG presentations are outstanding
  device_->waitForFences({acquireFence_}, VK_TRUE,
                         std::numeric_limits<uint64_t>::max());
  device_->resetFences({acquireFence_});

  auto resultValue = device_->acquireNextImageKHR(
      swapchain_.get(), std::numeric_limits<uint64_t>::max(),
      acquireCompleteSemaphore_.get(), acquireFence_);

  DEBUG_RUNTIME_ASSERT_TRUE(resultValue.result == vk::Result::eSuccess);
  currentPresentImageIndex = resultValue.value;

  vk::Flags stageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

  device_->waitForFences({swapchainImages_[currentPresentImageIndex].fence},
                         VK_TRUE, std::numeric_limits<uint64_t>::max());
  device_->resetFences({swapchainImages_[currentPresentImageIndex].fence});

  auto submitInfo = vk::SubmitInfo();
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &swapchainImages_[currentPresentImageIndex].cmd;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &acquireCompleteSemaphore_.get();
  submitInfo.pWaitDstStageMask = &stageMask;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &renderCompleteSemaphore_.get();

  deviceGraphicsQueue_.submit(submitInfo,
                              swapchainImages_[currentPresentImageIndex].fence);
}

void RenderingManagerVulkan::Present() const {
  auto presentInfo =
      vk::PresentInfoKHR(1, &renderCompleteSemaphore_.get(), 1,
                         &swapchain_.get(), &currentPresentImageIndex);

  devicePresentQueue_.presentKHR(presentInfo);
  devicePresentQueue_.waitIdle();
}
}  // namespace sce::rendering::private_
