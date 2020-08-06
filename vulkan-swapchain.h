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
        void createFramebuffers();

        VkSwapchainKHR getSwapchain();
        VkFormat getImageFormat();
        VkExtent2D getExtent();
        std::vector<VkFramebuffer>& getFramebuffers();
        size_t getImageCount();
    private:
        VkSwapchainKHR swapchain;
        VkFormat imageFormat;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;

        void retrieveImagesAndCreateImageViews();
    };
}
