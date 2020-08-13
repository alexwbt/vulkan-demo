#include "window.h"
#include "vulkan-state.h"
#include "vulkan-renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Vulkan
{
    Renderer::Renderer(CommandBuffer& commandBuffer, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, Texture& texture, Camera& camera)
        : commandBuffer{ commandBuffer }, vertexBuffer{ vertexBuffer }, indexBuffer{ indexBuffer }, texture{ texture }, camera{ camera }
    {
        createUniformBuffer();
        State::descriptorSetObj()->createDescriptorPool();
        State::descriptorSetObj()->createDescriptorSets(uniformBuffers, texture);
        runRenderCommands();

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

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        VkSubmitInfo submitInfo{};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        commandBuffer.submit(imageIndex, State::deviceObj()->getGraphicsQueue(), &submitInfo, inFlightFences[currentFrame]);

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

    void Renderer::runRenderCommands()
    {
        std::vector<VkDescriptorSet>& descriptorSets = State::descriptorSetObj()->getDescriptorSets();
        std::vector<VkFramebuffer>& framebuffers = State::pipelineObj()->getFramebuffers();
        for (size_t i = 0; i < commandBuffer.getSize(); i++)
        {
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = State::pipelineObj()->getRenderPass();
            renderPassInfo.framebuffer = framebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = State::swapchainObj()->getExtent();
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            VkCommandBuffer cb = commandBuffer.getBuffer(i);

            vkCmdBeginRenderPass(cb, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, State::getPipeline());

            VkBuffer vertexBuffers[] = { vertexBuffer.getBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(cb, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(cb, indexBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, State::pipelineObj()->getLayout(), 0, 1, &descriptorSets[i], 0, nullptr);

            vkCmdDrawIndexed(cb, static_cast<uint32_t>(indexBuffer.getIndices().size()), 1, 0, 0, 0);

            vkCmdEndRenderPass(cb);

            commandBuffer.end(i);
        }
    }

    void Renderer::resized()
    {
        while (Window::width == 0 || Window::height == 0)
            glfwWaitEvents();
        vkDeviceWaitIdle(State::getDevice());
        State::descriptorSetObj()->destroyDescriptorPool();
        commandBuffer.free();
        State::commandPoolObj()->destroy();
        State::pipelineObj()->destroy();
        State::getDepthImage().destroy();
        State::getDepthImage().setSize(Window::width, Window::height);
        State::swapchainObj()->destroy();
        State::swapchainObj()->create();
        State::getDepthImage().create();
        State::pipelineObj()->create();
        State::commandPoolObj()->create();
        commandBuffer.allocate();
        destroyUniformBuffer();
        createUniformBuffer();
        State::descriptorSetObj()->createDescriptorPool();
        State::descriptorSetObj()->createDescriptorSets(uniformBuffers, texture);
        runRenderCommands();
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
        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0f, 0.0f));
        // ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = camera.viewMatrix;
        ubo.projection = glm::perspective(glm::radians(camera.fov), Window::width / (float)Window::height, 0.001f, 100.0f);
        ubo.projection[1][1] *= -1;
        ubo.pvm = ubo.projection * ubo.view * ubo.model;

        uniformBuffers[currentImage]->storeData(&ubo);
    }
}
