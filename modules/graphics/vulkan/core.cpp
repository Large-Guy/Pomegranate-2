#include "core.h"

void Graphics::init() {
    Debug::AssertIf::isFalse(glfwInit(), "Failed to initialize GLFW");
}