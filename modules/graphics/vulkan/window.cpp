#include "window.h"

Window::Window() {
    this->_title = "Pomegranate Engine";
    this->_size = {800, 600};
    this->_fullscreen = false;
    this->_visible = false;
    this->_open = true;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->_window = glfwCreateWindow(this->_size.x, this->_size.y, this->_title.data(), nullptr, nullptr);
    Debug::AssertIf::isFalse(glfwCreateWindowSurface(Graphics::_instance, this->_window, nullptr, &this->_surface) == VK_SUCCESS,"Failed to create window surface");
    glfwHideWindow(this->_window);
}

Window::~Window() {
    vkDestroySurfaceKHR(Graphics::_instance, this->_surface, nullptr);
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

void Window::destroy() {
    delete this;
}