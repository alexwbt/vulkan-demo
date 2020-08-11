#include "window.h"
#include "vulkan-state.h"
#include "vulkan-renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace Vulkan
{
    Renderer::Renderer(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer) : vertexBuffer{ vertexBuffer }, indexBuffer{ indexBuffer }
    {
        createUniformBuffer();
        State::descriptorSetObj()->createDescriptorPool();
        State::descriptorSetObj()->createDescriptorSets(uniformBuffers);
        State::pipelineObj()->beginRenderPass(vertexBuffer, indexBuffer);

        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(State::swapchainObj()->getImageCount(), VK_NULL_HANDLE);
        currentFrame = 0;

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            if (vkCreateSemaphore(State::getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(State::getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(State::getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
                throw std::runtime_error("Failed to create synchronization objects for a frame.");
    }

    Renderer::~Renderer()
    {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(State::getDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(State::getDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(State::getDevice(), inFlightFences[i], nullptr);
        }
        destroyUniformBuffer();
    }

    void Renderer::render()
    {
        vkWaitForFences(State::getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(State::getDevice(), State::getSwapchain(), UINT64_MAX,
            imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            resized();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire swapchain image.");

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(State::getDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        vkResetFences(State::getDevice(), 1, &inFlightFences[currentFrame]);

        updateUniformBuffer(imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &State::commandPoolObj()->getCommandBuffers()[imageIndex];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(State::deviceObj()->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
            throw std::runtime_error("Failed to submit command buffer.");

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { State::getSwapchain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(State::deviceObj()->getPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window::resized)
        {
            Window::resized = false;
            resized();
        }
        else if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to present swapchain image.");

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::resized()
    {
        while (Window::width == 0 || Window::height == 0)
            glfwWaitEvents();
        vkDeviceWaitIdle(State::getDevice());
        State::descriptorSetObj()->destroyDescriptorPool();
        State::commandPoolObj()->destroy();
        State::pipelineObj()->destroy();
        State::swapchainObj()->destroy();
        State::swapchainObj()->create();
        State::pipelineObj()->create();
        State::commandPoolObj()->create();
        destroyUniformBuffer();
        createUniformBuffer();
        State::descriptorSetObj()->createDescriptorPool();
        State::descriptorSetObj()->createDescriptorSets(uniformBuffers);
        State::pipelineObj()->beginRenderPass(vertexBuffer, indexBuffer);
        APPLICATION_LOG("Resized.");
    }

    void Renderer::createUniformBuffer()
    {
        uniformBuffers.resize(State::swapchainObj()->getImageCount());
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        for (size_t i = 0; i < uniformBuffers.size(); i++)
            uniformBuffers[i] = new Buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    void Renderer::destroyUniformBuffer()
    {
        for (size_t i = 0; i < uniformBuffers.size(); i++)
            delete uniformBuffers[i];
    }

    void Renderer::updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.projection = glm::perspective(glm::radians(45.0f), Window::width / (float)Window::height, 0.1f, 10.0f);
        ubo.projection[1][1] *= -1;
        ubo.pvm = ubo.projection * ubo.view * ubo.model;

        void* data;
        vkMapMemory(State::getDevice(), uniformBuffers[currentImage]->getBufferMemory(), 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(State::getDevice(), uniformBuffers[currentImage]->getBufferMemory());
    }
}
