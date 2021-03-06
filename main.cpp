#include "vulkan-state.h"
#include "vulkan-renderer.h"
#include "vulkan-texture.h"
#include "window.h"
#include "camera.h"
#include "camera-input.h"
#include "model.h"

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
        DepthImage depthImage;
        Pipeline pipeline;
        CommandPool commandPool;
        CommandBuffer commandBuffer(swapchain.getImageCount(), 0);

        Camera camera;
        CameraInput cameraInput(Window::window, camera);

        Model model("models/viking_room.obj");
        VertexBuffer vertexBuffer(model.vertices);
        IndexBuffer indexBuffer(model.indices);
        Texture texture("textures/viking_room.png", VK_FORMAT_R8G8B8A8_SRGB);
        Renderer renderer(commandBuffer, vertexBuffer, indexBuffer, texture, camera);

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
