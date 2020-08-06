#include "window.h"
#include "vulkan-state.h"
#include "vulkan-renderer.h"

#include <iostream>

using namespace Vulkan;

int main()
{
    try
    {
        Window::initialize(800, 600, "Vulkan Demo");

        Instance instance;
        Surface surface;
        PhysicalDevice physicalDevice;
        Device device;
        Swapchain swapchain;
        Pipeline pipeline;
        CommandPool commandPool;

        std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}}
        };
        VertexBuffer vertexBuffer(vertices);

        pipeline.beginRenderPass(vertexBuffer);

        Renderer renderer;

        while (Window::update())
        {
            renderer.render(vertexBuffer);
        }

        vkDeviceWaitIdle(device.getDevice());

        Window::terminate();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
