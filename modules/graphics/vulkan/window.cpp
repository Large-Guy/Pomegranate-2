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

#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = GetModuleHandle(nullptr);
    createInfo.hwnd = glfwGetWin32Window(this->window);
    if (vkCreateWin32SurfaceKHR(Graphics::instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
#elif __linux__
    VkXlibSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.dpy = glfwGetX11Display();
    createInfo.window = glfwGetX11Window(this->window);
    if (vkCreateXlibSurfaceKHR(Graphics::instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
#elif __APPLE__
    VkMacOSSurfaceCreateInfoMVK createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    createInfo.pView = glfwGetCocoaWindow(this->window);
    if (vkCreateMacOSSurfaceMVK(Graphics::instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
#endif
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