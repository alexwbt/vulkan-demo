#pragma once

#include <vulkan/vulkan.h>

#include <optional>

namespace Vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);

    class PhysicalDevice
    {
    public:
        PhysicalDevice();

        VkPhysicalDevice getPhysicalDevice();
    private:
        VkPhysicalDevice physicalDevice;
    };
}
