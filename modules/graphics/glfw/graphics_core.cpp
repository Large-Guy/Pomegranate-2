#include "graphics_core.h"

int Graphics::_viewportWidth = 800;
int Graphics::_viewportHeight = 600;
int Graphics::_drawCalls = 0;
Texture2D* Graphics::pomegranateTexture = nullptr;

void Graphics::init() {
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

void Graphics::setRenderTexture(Texture2D *texture) {
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

void Graphics::setViewport(int width, int height) {
    glViewport(0, 0, width, height);
    _viewportWidth = width;
    _viewportHeight = height;
}

int Graphics::getViewportWidth() {
    return _viewportWidth;
}

int Graphics::getViewportHeight() {
    return _viewportHeight;
}

void Graphics::drawCall() {
    _drawCalls++;
}

int Graphics::getDrawCalls() {
    return _drawCalls;
}

void Graphics::clearDrawCalls() {
    _drawCalls = 0;
}