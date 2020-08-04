#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Swapchain
    {
    public:
        Swapchain();
        ~Swapchain();

        VkSwapchainKHR& getSwapchain();
    private:
        VkSwapchainKHR swapchain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
    };
}
