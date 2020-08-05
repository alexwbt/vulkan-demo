#pragma once

#include "configuration-macro.h"

#include "vulkan-instance.h"
#include "vulkan-surface.h"
#include "vulkan-physical-device.h"
#include "vulkan-device.h"
#include "vulkan-swapchain.h"
#include "vulkan-pipeline.h"

#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

#define STATE_GETTER_SETTER_H(name, member, returnType)\
static name* member##Obj();\
static returnType get##name();\
static void set##name(name* member);

namespace Vulkan
{
    const std::vector<const char*> requiredLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> requiredExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    class State
    {
    public:
        STATE_GETTER_SETTER_H(Instance, instance, VkInstance);
        STATE_GETTER_SETTER_H(Surface, surface, VkSurfaceKHR);
        STATE_GETTER_SETTER_H(PhysicalDevice, physicalDevice, VkPhysicalDevice);
        STATE_GETTER_SETTER_H(Device, device, VkDevice);
        STATE_GETTER_SETTER_H(Swapchain, swapchain, VkSwapchainKHR);
        STATE_GETTER_SETTER_H(Pipeline, pipeline, VkPipeline);

    private:
        static Instance* instance;
        static Surface* surface;
        static PhysicalDevice* physicalDevice;
        static Device* device;
        static Swapchain* swapchain;
        static Pipeline* pipeline;
    };
}
