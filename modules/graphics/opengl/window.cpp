#include "window.h"

Window::Window() {
    Graphics::getInstance();
    this->_title = "Pomegranate Engine";
    this->_size = {800, 600};
    this->_fullscreen = false;
    this->_visible = false;
    this->_open = true;
    this->_window = glfwCreateWindow(this->_size.x, this->_size.y, this->_title.data(), nullptr, nullptr);
    if(!this->_window) {
        //Get the error
        const char* error;
        glfwGetError(&error);
        Debug::Log::error("Failed to create window: " + std::string(error));
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(this->_window);

    Debug::AssertIf::isFalse(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

    glfwHideWindow(this->_window);
    draw.window = this;

    Graphics::_windows.push_back(this);
}

Window::~Window() {
    glfwDestroyWindow(this->_window);

    Graphics::_windows.erase(std::remove(Graphics::_windows.begin(), Graphics::_windows.end(), this), Graphics::getInstance()->_windows.end());
}

void Window::setTitle(const std::string& title) {
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

std::string Window::getTitle() const {
    return this->_title;
}

bool Window::isOpen() const {
    return this->_open;
}

//----------- Draw Class ------------

void Window::Draw::begin() {
    glfwMakeContextCurrent(this->window->_window);
}

void Window::Draw::end() {
    glfwSwapBuffers(this->window->_window);
}

void Window::Draw::clear(Vector4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/*
void Window::Draw::buffers(BufferBase<BUFFER_TYPE_VERTEX>* vertexBuffer, BufferBase<BUFFER_TYPE_INDEX>* indexBuffer, ShaderBase* shader) {

}
*/

void Window::Draw::shader(ShaderBase* shader) {
    _info = shader->_info;
    glUseProgram(shader->_program);
}

void Window::Draw::mesh(MeshBase* mesh) {
    _topologyMode = _info.topologyMode;
    if(_info.cullMode != CULL_MODE_NONE) {
        glEnable(GL_CULL_FACE);
        glCullFace(_info.cullMode);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, _info.renderMode);
    glBindVertexArray(mesh->_vao);
    glDrawElements(_topologyMode, mesh->getIndexCount(), GL_UNSIGNED_INT, nullptr);
}
