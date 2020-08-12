#pragma once

#include "vulkan-command-buffer.h"

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
    private:
        VkCommandPool commandPool;
    };
}
