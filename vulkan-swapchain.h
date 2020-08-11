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

        void create();
        void destroy();

        VkSwapchainKHR getSwapchain();
        VkFormat getImageFormat();
        VkExtent2D getExtent();
        size_t getImageCount();
        VkImageView getImageView(int i);
    private:
        VkSwapchainKHR swapchain;
        VkFormat imageFormat;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        void retrieveImagesAndCreateImageViews();
    };
}
