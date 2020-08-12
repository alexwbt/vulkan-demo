#pragma once

#include "vulkan-image-view.h"

#include <string>

namespace Vulkan
{
    class TextureImage
    {
    public:
        TextureImage(std::string path);
        ~TextureImage();

        VkImage getImage();
    private:
        int imageWidth, imageHeight, imageChannels;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;

        void createImage();
        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer);
    };
}
