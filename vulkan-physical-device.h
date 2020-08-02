#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class PhysicalDevice
    {
    public:
        PhysicalDevice();

        VkPhysicalDevice& getPhysicalDevice();
    private:
        VkPhysicalDevice physicalDevice;
    };
}
