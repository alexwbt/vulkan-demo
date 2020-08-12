#pragma once

#include "vulkan-vertex-buffer.h"
#include "vulkan-index-buffer.h"

namespace Vulkan
{
    class Pipeline
    {
    public:
        Pipeline();
        ~Pipeline();

        void create();
        void destroy();

        VkPipeline getPipeline();
        VkPipelineLayout getLayout();
        VkRenderPass getRenderPass();
        std::vector<VkFramebuffer>& getFramebuffers();
    private:
        VkPipeline pipeline;
        VkPipelineLayout layout;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> framebuffers;

        void createLayout();
        void createRenderPass();
        void createFramebuffers();
    };
}
