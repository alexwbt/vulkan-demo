#pragma once

#include "vulkan-vertex-buffer.h"

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Pipeline
    {
    public:
        Pipeline();
        ~Pipeline();

        void create();
        void destroy();
        void beginRenderPass(VertexBuffer& vertexBuffer);

        VkPipeline getPipeline();
        VkRenderPass getRenderPass();
    private:
        VkPipeline pipeline;
        VkPipelineLayout layout;
        VkRenderPass renderPass;

        void createLayout();
        void createRenderPass();
    };
}
