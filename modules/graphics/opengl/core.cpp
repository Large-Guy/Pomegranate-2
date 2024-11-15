#include "core.h"

Graphics Graphics::_instance{};
std::vector<Window*> Graphics::_windows{};

Graphics::Graphics() {
    // Initialize GLFW
    Debug::AssertIf::isFalse(glfwInit(), "Failed to initialize GLFW");
    Debug::Log("GLFW initialized");

    // Set the OpenGL version to 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

Graphics* Graphics::getInstance() {
    return &_instance;
}