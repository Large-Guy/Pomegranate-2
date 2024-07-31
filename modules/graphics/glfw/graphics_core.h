#ifndef POMEGRANATEENGINE_GRAPHICS_CORE_H
#define POMEGRANATEENGINE_GRAPHICS_CORE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <texture.h>

class Graphics {
private:
    static int _viewportWidth;
    static int _viewportHeight;
public:
    static void init();
    static void setRenderTexture(Texture2D* texture);
    static void setViewport(int width, int height);
    [[nodiscard]] static int getViewportWidth();
    [[nodiscard]] static int getViewportHeight();
};


#endif //POMEGRANATEENGINE_GRAPHICS_CORE_H
