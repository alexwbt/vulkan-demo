#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Instance
    {
    public:
        Instance();
        ~Instance();

        VkInstance getInstance();
    private:
        VkInstance instance;
    };
}
