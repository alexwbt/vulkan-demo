#pragma once

#include "vulkan-instance.h"
#include "vulkan-surface.h"
#include "vulkan-physical-device.h"

namespace Vulkan
{
    class State
    {
    public:
        static VkInstance& getInstance();
        static void setInstance(Instance* instance);

        static VkSurfaceKHR& getSurface();
        static void setSurface(Surface* instance);

        static VkPhysicalDevice& getPhysicalDevice();
        static void setPhysicalDevice(PhysicalDevice* instance);

    private:
        static Instance* instance;
        static Surface* surface;
        static PhysicalDevice* physicalDevice;
    };
}
