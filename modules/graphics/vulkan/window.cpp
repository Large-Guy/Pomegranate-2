#include "window.h"

Window::Window() {
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
}

Window::~Window() {
    glfwDestroyWindow(this->window);
}

void Window::show() {
    glfwShowWindow(this->window);
}

void Window::hide() {
    glfwHideWindow(this->window);
}

void Window::setTitle(const std::string& new_title) {
    this->title = new_title;
    glfwSetWindowTitle(this->window,new_title.c_str());
}

void Window::setSize(int new_width, int new_height) {
    this->width = new_width;
    this->height = new_height;
    glfwSetWindowSize(this->window,new_width,new_height);
}

void Window::setFullscreen(bool is_fullscreen) {
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

void Window::poll() {
    glfwPollEvents();
}

bool Window::is_open() {
    return !glfwWindowShouldClose(this->window);
}