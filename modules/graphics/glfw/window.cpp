#include "window.h"

Window::Window() {
    _window = nullptr;
}

Window::~Window() {
    if (_open) {
        close();
    }
}

void Window::setSize(int width, int height) {
    this->_width = width;
    this->_height = height;
}

void Window::setTitle(const char* title) {
    this->_title = title;
}

void Window::open() {
    _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
    }
    _open = true;

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
    }

    draw._window = _window;
}

void Window::close() {
    glfwDestroyWindow(_window);
    _open = false;
}

bool Window::isOpen() const {
    return _open;
}

int Window::getWidth() const {
    return _width;
}

int Window::getHeight() const {
    return _height;
}

void Window::pollEvents() {
    glfwPollEvents();
    //Close window if close button is pressed
    if (glfwWindowShouldClose(_window)) {
        close();
    }
}

void Window::Draw::begin() {
    glfwMakeContextCurrent(_window);
    _color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::Draw::end() {
    glfwSwapBuffers(_window);
}

void Window::Draw::clear() const {
    glClearColor(_color.x,_color.y,_color.z,_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Draw::setColor(Vector4 color) {
    _color = color;
}

void Window::Draw::setColor(Vector3 color, float a) {
    _color = Vector4(color.x, color.y, color.z, a);
}
