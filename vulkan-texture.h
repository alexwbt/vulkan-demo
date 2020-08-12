#pragma once

#include "vulkan-texture-image.h"

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
        TextureImage image;
        ImageView imageView;
        VkSampler sampler;

        void createSampler();
    };
}
