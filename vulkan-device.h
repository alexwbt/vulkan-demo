#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Device
    {
    public:
        Device();
        ~Device();

        VkDevice& getDevice();
    private:
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
    };
}
