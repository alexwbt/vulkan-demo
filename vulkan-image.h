#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace Vulkan
{
    class Image
    {
    public:
        static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        Image(int width, int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
        Image(std::string path, VkFormat format, VkImageAspectFlags aspectFlags);
        ~Image();

        void setSize(int width, int height);
        void create();
        void destroy();

        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer);

        VkFormat getFormat();
        VkImageView getView();
    private:
        int width, height, channels;
        VkImage image;
        VkDeviceMemory memory;
        VkFormat format;
        VkImageView view;
        VkImageAspectFlags aspectFlags;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags properties;

        void createImage();
        void createImageView();
    };
}
