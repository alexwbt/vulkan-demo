#pragma once

#include <vulkan/vulkan.h>

#include <vector>

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
        VkFormat imageFormat;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        void retrieveImagesAndCreateImageViews();
    };
}
