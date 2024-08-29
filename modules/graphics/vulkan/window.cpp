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
        std::cout << "Error: " << code << " Description: " << description << std::endl;
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

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    Graphics::QueueFamilies indices = Graphics::getQueueFamilies(Graphics::physicalDevice,surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),indices.presentFamily.value()};

    if(indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(Graphics::logicalDevice,&createInfo, nullptr,&swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }
}

PomegranateWindow::~PomegranateWindow() {
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