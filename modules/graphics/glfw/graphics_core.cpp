#include "graphics_core.h"

int GraphicsCore::_viewportWidth = 800;
int GraphicsCore::_viewportHeight = 600;

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

void GraphicsCore::setRenderTexture(Texture2D *texture) {
    if(texture == nullptr)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 800, 600);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture->getGLTexture());
    glViewport(0, 0, texture->getWidth(), texture->getHeight());
    _viewportWidth = texture->getWidth();
    _viewportHeight = texture->getHeight();
}

void GraphicsCore::setViewport(int width, int height) {
    glViewport(0, 0, width, height);
    _viewportWidth = width;
    _viewportHeight = height;
}

int GraphicsCore::getViewportWidth() {
    return _viewportWidth;
}

int GraphicsCore::getViewportHeight() {
    return _viewportHeight;
}