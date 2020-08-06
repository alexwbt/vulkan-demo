#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Device
    {
    public:
        Device();
        ~Device();

        VkDevice getDevice();
        VkQueue getPresentQueue();
        VkQueue getGraphicsQueue();
    private:
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
    };
}
