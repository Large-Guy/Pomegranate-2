#include "window.h"
#include "shader.h"

void Window::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = Graphics::getInstance()->getSwapChainSupport(Graphics::getInstance()->_physicalDevice, &_surface);

    VkSurfaceFormatKHR surfaceFormat = Graphics::getInstance()->getSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = Graphics::getInstance()->chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = getExtents(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = Graphics::getInstance()->getQueueFamilies(Graphics::getInstance()->_physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if(indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = true;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    Debug::AssertIf::isFalse(vkCreateSwapchainKHR(Graphics::getInstance()->_logicalDevice,&createInfo, nullptr,&_swapChain) == VK_SUCCESS, "Failed to create swap chain!");

    Debug::Log::pass("Created swap chain!");

    vkGetSwapchainImagesKHR(Graphics::getInstance()->_logicalDevice,_swapChain,&imageCount, nullptr);
    _swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(Graphics::getInstance()->_logicalDevice, _swapChain, &imageCount, _swapChainImages.data());

    _swapChainImageFormat = surfaceFormat.format;
    _swapExtent = extent;

    Debug::Log::pass("Retrieved swap chain images");
}

void Window::createImageViews() {
    _swapChainImageViews.resize(_swapChainImages.size());
    for(size_t i = 0; i < _swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        Debug::AssertIf::isFalse(vkCreateImageView(Graphics::getInstance()->_logicalDevice,&createInfo, nullptr,&_swapChainImageViews[i]) == VK_SUCCESS, "Failed to create image views!");
    }
    Debug::Log::pass("Created image views");
}

void Window::createFramebuffers() {
    _swapChainFramebuffers.resize(_swapChainImageViews.size());

    for (int i = 0; i < _swapChainImageViews.size(); ++i) {
        VkImageView attachments[] = {
                _swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = _renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = attachments;
        framebufferCreateInfo.width = _swapExtent.width;
        framebufferCreateInfo.height = _swapExtent.height;
        framebufferCreateInfo.layers = 1;

        Debug::AssertIf::isFalse(vkCreateFramebuffer(Graphics::getInstance()->_logicalDevice, &framebufferCreateInfo,
                                                     nullptr,&_swapChainFramebuffers[i]) == VK_SUCCESS, "Failed to create framebuffer");
    }
    Debug::Log::pass("Successfully created frame buffers");
}

void Window::createCommandBuffer() {
    _commandBuffers.resize(Graphics::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = Graphics::getInstance()->_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = _commandBuffers.size();

    Debug::AssertIf::isFalse(vkAllocateCommandBuffers(Graphics::getInstance()->_logicalDevice, &allocInfo, _commandBuffers.data()) == VK_SUCCESS, "Failed to allocate command buffers!");
}

void Window::Draw::drawBuffers(Buffer<Vertex2D>& vertexBuffer, Buffer<uint16_t>& indexBuffer, ShaderBase* shader) {
    VkCommandBuffer& commandBuffer = window->getCurrentCommandBuffer();

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(window->_swapExtent.width);
    viewport.height = static_cast<float>(window->_swapExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = window->_swapExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,shader->_pipelines[window].pipeline);

    VkBuffer vertexBuffers[] = {vertexBuffer._buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer,indexBuffer._buffer,0,VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(commandBuffer,(uint32_t)indexBuffer._data.size(),1,0,0,0);
}

void Window::Draw::drawBuffers(Buffer<Vertex3D>& vertexBuffer, Buffer<uint16_t>& indexBuffer, ShaderBase* shader) {
    VkCommandBuffer& commandBuffer = window->getCurrentCommandBuffer();

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(window->_swapExtent.width);
    viewport.height = static_cast<float>(window->_swapExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = window->_swapExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,shader->_pipelines[window].pipeline);

    VkBuffer vertexBuffers[] = {vertexBuffer._buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer,indexBuffer._buffer,0,VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(commandBuffer,(uint32_t)indexBuffer._data.size(),1,0,0,0);
}

void Window::beginCommandBuffer() {
    VkCommandBuffer& commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    Debug::AssertIf::isFalse(vkBeginCommandBuffer(commandBuffer,&beginInfo) == VK_SUCCESS, "Failed to begin recording command buffer!");
}

void Window::endCommandBuffer() {
    vkCmdEndRenderPass(getCurrentCommandBuffer());

    Debug::AssertIf::isFalse(vkEndCommandBuffer(getCurrentCommandBuffer()) == VK_SUCCESS, "Failed to end command buffer!");
}

VkExtent2D Window::getExtents(const VkSurfaceCapabilitiesKHR &capabilities) {
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(_window,&width,&height);

        VkExtent2D actualExtent = {
                (uint32_t)width,
                (uint32_t)height
        };

        actualExtent.width = std::clamp(actualExtent.width,capabilities.minImageExtent.width,capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,capabilities.minImageExtent.height,capabilities.maxImageExtent.height);
        return actualExtent;
    }
}

VkCommandBuffer& Window::getCurrentCommandBuffer() {
    return _commandBuffers[_currentFrame];
}

Window::Window() {
    Graphics::getInstance();
    Graphics::getInstance()->_windows.push_back(this);
    this->_title = "Pomegranate Engine";
    this->_size = {800, 600};
    this->_fullscreen = false;
    this->_visible = false;
    this->_open = true;
    this->_window = glfwCreateWindow(this->_size.x, this->_size.y, this->_title.data(), nullptr, nullptr);
    Debug::AssertIf::isFalse(glfwCreateWindowSurface(Graphics::getInstance()->_instance, this->_window, nullptr, &this->_surface) == VK_SUCCESS,"Failed to create window surface");
    glfwHideWindow(this->_window);
    createSwapChain();
    createImageViews();
    Graphics::getInstance()->createRenderPass(this);
    createFramebuffers();
    createCommandBuffer();
    for(auto shader : Graphics::getInstance()->_shaders) {
        shader->requestPipeline(this);
    }
    draw.window = this;
}

Window::~Window() {
    for(auto framebuffer : _swapChainFramebuffers) {
        vkDestroyFramebuffer(Graphics::getInstance()->_logicalDevice,framebuffer, nullptr);
    }

    for(auto imageView : _swapChainImageViews)
    {
        vkDestroyImageView(Graphics::getInstance()->_logicalDevice,imageView, nullptr);
    }
    vkDestroySwapchainKHR(Graphics::getInstance()->_logicalDevice,_swapChain, nullptr);
    vkDestroySurfaceKHR(Graphics::getInstance()->_instance, this->_surface, nullptr);
    vkDestroyRenderPass(Graphics::getInstance()->_logicalDevice,_renderPass, nullptr);
    glfwDestroyWindow(this->_window);

    //Remove from windows list
    Graphics::getInstance()->_windows.erase(std::remove(Graphics::getInstance()->_windows.begin(), Graphics::getInstance()->_windows.end(), this), Graphics::getInstance()->_windows.end());
}

void Window::setTitle(const String& title) {
    this->_title = title;
    glfwSetWindowTitle(this->_window, this->_title.data());
}

void Window::setSize(Vector2i size) {
    this->_size = size;
    glfwSetWindowSize(this->_window, this->_size.x, this->_size.y);
}

void Window::setSize(int width, int height) {
    this->_size = {width, height};
    glfwSetWindowSize(this->_window, this->_size.x, this->_size.y);
}

void Window::poll() {
    glfwPollEvents();
    if(glfwWindowShouldClose(this->_window)) {
        this->_open = false;
    }
    else {
        this->_open = true;
    }
    this->_position;
    glfwGetWindowPos(this->_window, &this->_position.x, &this->_position.y);
}

void Window::show() {
    glfwShowWindow(this->_window);
    this->_visible = true;
}

void Window::hide() {
    glfwHideWindow(this->_window);
    this->_visible = false;
}

void Window::fullscreen() {
    if(this->_fullscreen) {
        glfwSetWindowMonitor(this->_window, nullptr, 0, 0, this->_size.x, this->_size.y, 0);
        this->_fullscreen = false;
    }
    else {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(this->_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        this->_fullscreen = true;
    }
}

void Window::maximize() {
    glfwMaximizeWindow(this->_window);
}

void Window::minimize() {
    glfwIconifyWindow(this->_window);
}

void Window::focus() {
    glfwFocusWindow(this->_window);
}

void Window::setPosition(Vector2i position) {
    this->_position = position;
    glfwSetWindowPos(this->_window, this->_position.x, this->_position.y);
}

void Window::setPosition(int x, int y) {
    this->_position = {x, y};
    glfwSetWindowPos(this->_window, this->_position.x, this->_position.y);
}

Vector2i Window::getPosition() const {
    return this->_position;
}

Vector2i Window::getSize() const {
    return this->_size;
}

String Window::getTitle() const {
    return this->_title;
}

bool Window::isOpen() const {
    return this->_open;
}

//----------- Draw Class ------------

void Window::Draw::begin() {
    vkWaitForFences(Graphics::getInstance()->_logicalDevice, 1, &Graphics::getInstance()->inFlightFences[window->_currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(Graphics::getInstance()->_logicalDevice,1,&Graphics::getInstance()->inFlightFences[window->_currentFrame]);

    vkAcquireNextImageKHR(Graphics::getInstance()->_logicalDevice, window->_swapChain, UINT64_MAX, Graphics::getInstance()->imageAvailableSemaphores[window->_currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(window->getCurrentCommandBuffer(), 0);

    window->beginCommandBuffer();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = window->_renderPass;
    renderPassInfo.framebuffer = window->_swapChainFramebuffers[window->draw.imageIndex];
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = window->_swapExtent;

    renderPassInfo.clearValueCount = 0;
    renderPassInfo.pClearValues = nullptr;

    vkCmdBeginRenderPass(window->getCurrentCommandBuffer(),&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Window::Draw::end() {
    window->endCommandBuffer();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {Graphics::getInstance()->imageAvailableSemaphores[window->_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &window->getCurrentCommandBuffer();

    VkSemaphore signalSemaphores[] = {Graphics::getInstance()->renderFinishedSemaphores[window->_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    Debug::AssertIf::isFalse(vkQueueSubmit(Graphics::getInstance()->_queues.graphicsQueue,1,&submitInfo,Graphics::getInstance()->inFlightFences[window->_currentFrame]) == VK_SUCCESS, "Failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {window->_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(Graphics::getInstance()->_queues.presentQueue, &presentInfo);

    window->_currentFrame = (window->_currentFrame + 1) % Graphics::MAX_FRAMES_IN_FLIGHT;
}

void Window::Draw::clear(Vector4 color) {
    VkClearValue clearColor = {color.x, color.y, color.z, color.w};

    VkClearAttachment clearAttachment{};
    clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clearAttachment.colorAttachment = 0;
    clearAttachment.clearValue = clearColor;

    VkClearRect clearRect{};
    clearRect.layerCount = 1;
    clearRect.rect.offset = {0,0};
    clearRect.rect.extent = window->_swapExtent;

    vkCmdClearAttachments(window->getCurrentCommandBuffer(), 1, &clearAttachment, 1, &clearRect);
}