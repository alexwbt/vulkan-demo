#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Surface
    {
    public:
        Surface();
        ~Surface();

        VkSurfaceKHR getSurface();
    private:
        VkSurfaceKHR surface;
    };
}
