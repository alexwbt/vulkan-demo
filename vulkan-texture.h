#pragma once

#include "vulkan-image.h"

namespace Vulkan
{
    class Texture
    {
    public:
        Texture(std::string path, VkFormat format);
        ~Texture();

        VkImageView getImageView();
        VkSampler getSampler();
    private:
        Image* image;
        VkSampler sampler;

        void createSampler();
    };
}
