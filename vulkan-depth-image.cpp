#include "vulkan-state.h"
#include "window.h"

namespace Vulkan
{
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(State::getPhysicalDevice() , format, &props);
            if ((tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) ||
                (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features))
                return format;
        }
        throw std::runtime_error("Failed to find supported format.");
    }

    VkFormat findDepthFormat() {
        return findSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    DepthImage::DepthImage() :
        depthImage(Window::width, Window::height, findDepthFormat(),
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)
    {
        State::setDepthImage(this);
        APPLICATION_LOG("Created Depth Image.");
    }

    DepthImage::~DepthImage()
    {
        APPLICATION_LOG("Destroyed Depth Image.");
    }

    Image& DepthImage::getDepthImage()
    {
        return depthImage;
    }
}
