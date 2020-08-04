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
        VkQueue* getGraphicsQueue();
        VkQueue* getPresentQueue();
    private:
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
    };
}
