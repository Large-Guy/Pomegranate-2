#include "window.h"

PomegranateWindow::PomegranateWindow() {
    this->width = 800;
    this->height = 600;
    this->fullscreen = false;
    this->title = "Pomegranate Engine";
    this->window = glfwCreateWindow(width,height,title.c_str(), nullptr, nullptr);
    this->visible = false;
    this->close_requested = false;

    if (!window) {
        //Get error
        const char *description;
        int code = glfwGetError(&description);
        Debug::Log::error(String("Error: ") + String(code) + String(" Description: ") + description);
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwHideWindow(this->window);
    if (glfwCreateWindowSurface(Graphics::instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    Graphics::SwapChainSupportDetails swapChainSupport = Graphics::getSwapChainSupport(Graphics::physicalDevice,surface);

    VkSurfaceFormatKHR surfaceFormat = Graphics::chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = Graphics::chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = Graphics::chooseSwapExtent(swapChainSupport.capabilities,window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    Graphics::QueueFamilies indices = Graphics::getQueueFamilies(Graphics::physicalDevice,surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),indices.presentFamily.value()};

    if(indices.graphicsFamily != indices.presentFamily) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(Graphics::logicalDevice, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); ++i) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(Graphics::logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

PomegranateWindow::~PomegranateWindow() {
    for(auto imageView : swapChainImageViews) {
        vkDestroyImageView(Graphics::logicalDevice, imageView, nullptr);
    }
    glfwDestroyWindow(this->window);
}

void PomegranateWindow::show() {
    glfwShowWindow(this->window);
}

void PomegranateWindow::hide() {
    glfwHideWindow(this->window);
}

void PomegranateWindow::setTitle(const std::string& new_title) {
    this->title = new_title;
    glfwSetWindowTitle(this->window,new_title.c_str());
}

void PomegranateWindow::setSize(int new_width, int new_height) {
    this->width = new_width;
    this->height = new_height;
    glfwSetWindowSize(this->window,new_width,new_height);
}

void PomegranateWindow::setFullscreen(bool is_fullscreen) {
    this->fullscreen = is_fullscreen;
    if(this->fullscreen)
    {
        //Get the primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //Get the video mode of the monitor
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        //Set the window to fullscreen
        glfwSetWindowMonitor(this->window,monitor,0,0,mode->width,mode->height,mode->refreshRate);
    }
    else
    {
        //Get the primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //Get the video mode of the monitor
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        //Set the window to fullscreen
        glfwSetWindowMonitor(this->window,nullptr,0,0,this->width,this->height,mode->refreshRate);
    }
}

void PomegranateWindow::poll() {
    glfwPollEvents();
}

bool PomegranateWindow::is_open() {
    return !glfwWindowShouldClose(this->window);
}