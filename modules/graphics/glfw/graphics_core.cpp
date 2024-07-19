#include "graphics_core.h"

void GraphicsCore::init() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        //Get error
        const char *description;
        int code = glfwGetError(&description);
        std::cout << "Error: " << code << " Description: " << description << std::endl;
    }
    //Set GL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
}