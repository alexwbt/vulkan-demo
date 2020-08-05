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

        VkSwapchainKHR getSwapchain();
        VkFormat getImageFormat();
        VkExtent2D getExtent();
    private:
        VkSwapchainKHR swapchain;
        VkFormat imageFormat;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        void retrieveImagesAndCreateImageViews();
    };
}
