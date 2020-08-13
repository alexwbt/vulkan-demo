#include "vulkan-image.h"
#include "vulkan-state.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Vulkan
{
    Image::Image(std::string path, VkFormat format, VkImageAspectFlags aspectFlags)
        : format(format), aspectFlags(aspectFlags)
    {
        stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) throw std::runtime_error("Failed to load texture image.");

        int size = width * height * 4;
        Buffer stagingBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingBuffer.storeData(pixels);

        stbi_image_free(pixels);

        tiling = VK_IMAGE_TILING_OPTIMAL;
        usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        createImage();
        createImageView();

        CommandBuffer commandBuffer(1, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        transitionImageLayout(commandBuffer.getBuffer(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(commandBuffer.getBuffer(0), stagingBuffer.getBuffer());
        transitionImageLayout(commandBuffer.getBuffer(0), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        commandBuffer.end();
        commandBuffer.submit();
        commandBuffer.wait();
    }

    Image::Image(int width, int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags)
        : width(width), height(height), format(format), tiling(tiling), usage(usage), properties(properties), aspectFlags(aspectFlags)
    {
        create();
    }

    Image::~Image()
    {
        destroy();
    }

    void Image::setSize(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    void Image::create()
    {
        createImage();
        createImageView();
    }

    void Image::destroy()
    {
        vkDestroyImageView(State::getDevice(), view, nullptr);
        vkDestroyImage(State::getDevice(), image, nullptr);
        vkFreeMemory(State::getDevice(), memory, nullptr);
    }

    void Image::createImage()
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateImage(State::getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
            throw std::runtime_error("Failed to create image.");

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(State::getDevice(), image, &memRequirements);
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(State::getDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate image memory.");
        vkBindImageMemory(State::getDevice(), image, memory, 0);
    }

    void Image::createImageView()
    {
        view = createImageView(image, format, aspectFlags);
    }

    VkImageView Image::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
    {
        VkImageView view;
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(State::getDevice(), &viewInfo, nullptr, &view) != VK_SUCCESS)
            throw std::runtime_error("Failed to create image view.");
        return view;
    }

    void Image::transitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else throw std::invalid_argument("Unsupported layout transition.");

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    void Image::copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer)
    {
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }

    VkFormat Image::getFormat()
    {
        return format;
    }

    VkImageView Image::getView()
    {
        return view;
    }
}
