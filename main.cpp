#include "vulkan-state.h"
#include "vulkan-renderer.h"
#include "window.h"
#include "camera.h"
#include "camera-input.h"

#include <chrono>
#include <iostream>

using namespace Vulkan;

int main()
{
    try
    {
        Window::initialize(1200, 720, "Vulkan Demo");

        Instance instance;
        Surface surface;
        PhysicalDevice physicalDevice;
        Device device;
        Swapchain swapchain;
        DescriptorSet descriptorSet;
        Pipeline pipeline;
        CommandPool commandPool;

        Camera camera;
        CameraInput cameraInput(Window::window, camera);

        std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };
        std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
        };
        VertexBuffer vertexBuffer(vertices);
        IndexBuffer indexBuffer(indices);
        Renderer renderer(vertexBuffer, indexBuffer, camera);

        float fpsCounter = 0;
        int fps = 0;
        std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        while (Window::update())
        {
            std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
            fps++;
            fpsCounter += deltaTime;
            if (fpsCounter >= 1)
            {
                Window::setTitle(Window::title + std::string(" | FPS:") + std::to_string(fps));
                fps = 0;
                fpsCounter = 0;
            }

            cameraInput.update(deltaTime);

            renderer.render();

            startTime = currentTime;
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
