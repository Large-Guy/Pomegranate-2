#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>

int main() {

    Graphics::enableValidationLayers = true;

    auto vertexFile = File("assets/graphics/shaders/shader.vert.spv");
    vertexFile.open();
    auto fragmentFile = File("assets/graphics/shaders/shader.frag.spv");
    fragmentFile.open();

    auto vertexShader = vertexFile.readBuffer();
    auto fragmentShader = fragmentFile.readBuffer();

    Shader shader(vertexShader, fragmentShader);

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    while(window.isOpen()) {
        window.poll();

        vkWaitForFences(Graphics::getInstance()->_logicalDevice, 1, &Graphics::getInstance()->inFlightFences[window._currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(Graphics::getInstance()->_logicalDevice,1,&Graphics::getInstance()->inFlightFences[window._currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(Graphics::getInstance()->_logicalDevice, window._swapChain, UINT64_MAX, Graphics::getInstance()->imageAvailableSemaphores[window._currentFrame], VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(window._commandBuffers[window._currentFrame], 0);
        window.recordCommandBuffer(window._commandBuffers[window._currentFrame], imageIndex, &shader);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {Graphics::getInstance()->imageAvailableSemaphores[window._currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &window._commandBuffers[window._currentFrame];

        VkSemaphore signalSemaphores[] = {Graphics::getInstance()->renderFinishedSemaphores[window._currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        Debug::AssertIf::isFalse(vkQueueSubmit(Graphics::getInstance()->_queues.graphicsQueue,1,&submitInfo,Graphics::getInstance()->inFlightFences[window._currentFrame]) == VK_SUCCESS, "Failed to submit draw command buffer!");

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {window._swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        vkQueuePresentKHR(Graphics::getInstance()->_queues.presentQueue, &presentInfo);

        window._currentFrame = (window._currentFrame + 1) % Graphics::MAX_FRAMES_IN_FLIGHT;
    }

    return 0;
}