#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class CommandPool
    {
    public:
        CommandPool();
        ~CommandPool();

        void create();
        void destroy();

        VkCommandPool getCommandPool();
        std::vector<VkCommandBuffer>& getCommandBuffers();
    private:
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        void allocateCommandBuffers();
    };
}
