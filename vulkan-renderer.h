#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Vulkan
{
    class Renderer
    {
    public:
        Renderer(VertexBuffer& vertexBuffer);
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
    };
}
