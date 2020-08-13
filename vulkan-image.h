#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace Vulkan
{
    class Image
    {
    public:
        Image(std::string path, VkFormat format);
        Image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        ~Image();

        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer);

        VkImageView getView();
    private:
        int width, height, channels;
        VkImage image;
        VkDeviceMemory memory;
        VkFormat format;
        VkImageView view;

        void createImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    };
}
