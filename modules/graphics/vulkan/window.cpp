#include "window.h"

void Window::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = Graphics::getInstance()->getSwapChainSupport(Graphics::getInstance()->_physicalDevice);

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

Window::Window() {
    Graphics::getInstance();
    this->_title = "Pomegranate Engine";
    this->_size = {800, 600};
    this->_fullscreen = false;
    this->_visible = false;
    this->_open = true;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->_window = glfwCreateWindow(this->_size.x, this->_size.y, this->_title.data(), nullptr, nullptr);
    Debug::AssertIf::isFalse(glfwCreateWindowSurface(Graphics::getInstance()->_instance, this->_window, nullptr, &this->_surface) == VK_SUCCESS,"Failed to create window surface");
    glfwHideWindow(this->_window);
    Graphics::getInstance()->_currentSurface = this->_surface; //Set the current surface to the window surface
    Graphics::getInstance()->createPhysicalDevice(); //Create the physical device now because we need the surface to do so
    Graphics::getInstance()->createLogicalDevice(Graphics::enableValidationLayers); //Create the logical device now because we need the physical device to do so
    createSwapChain();
    createImageViews();
}

Window::~Window() {
    for(auto imageView : _swapChainImageViews)
    {
        vkDestroyImageView(Graphics::getInstance()->_logicalDevice,imageView, nullptr);
    }

    vkDestroySwapchainKHR(Graphics::getInstance()->_logicalDevice,_swapChain, nullptr);
    vkDestroySurfaceKHR(Graphics::getInstance()->_instance, this->_surface, nullptr);
    glfwDestroyWindow(this->_window);
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