#include "vulkan-texture.h"
#include "vulkan-state.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Vulkan
{
    Texture::Texture(std::string path, VkFormat format)
    {
        int width, height, channels;
        stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) throw std::runtime_error("Failed to load texture image.");

        int size = width * height * 4;
        Buffer stagingBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingBuffer.storeData(pixels);

        stbi_image_free(pixels);

        image = new Image(width, height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        CommandBuffer commandBuffer(1, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        image->transitionImageLayout(commandBuffer.getBuffer(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        image->copyBufferToImage(commandBuffer.getBuffer(0), stagingBuffer.getBuffer());
        image->transitionImageLayout(commandBuffer.getBuffer(0), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        commandBuffer.end();
        commandBuffer.submit();
        commandBuffer.wait();

        createSampler();
    }

    Texture::~Texture()
    {
        vkDestroySampler(State::getDevice(), sampler, nullptr);
        delete image;
    }

    void Texture::createSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        if (vkCreateSampler(State::getDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
            throw std::runtime_error("Failed to create texture sampler.");
    }

    VkImageView Texture::getImageView()
    {
        return image->getView();
    }

    VkSampler Texture::getSampler()
    {
        return sampler;
    }
}
