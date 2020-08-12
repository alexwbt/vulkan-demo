#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class ImageView
    {
    public:
        ImageView(VkImage image, VkFormat format);
        ~ImageView();

        VkImageView getImageView();
    private:
        VkImageView imageView;
    };
}
