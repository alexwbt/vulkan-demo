#pragma once

#include "vulkan-vertex-buffer.h"
#include "vulkan-index-buffer.h"

namespace Vulkan
{
    class Renderer
    {
    public:
        Renderer(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);
        ~Renderer();
        
        void render();
    private:
        const int MAX_FRAMES_IN_FLIGHT = 2;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame;

        VertexBuffer& vertexBuffer;
        IndexBuffer& indexBuffer;

        void resized();
    };
}
