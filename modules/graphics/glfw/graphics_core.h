#ifndef POMEGRANATEENGINE_GRAPHICS_CORE_H
#define POMEGRANATEENGINE_GRAPHICS_CORE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "texture.h"

class Graphics {
private:
    static int _viewportWidth;
    static int _viewportHeight;
    static int _drawCalls;
public:
    static Texture2D* pomegranateTexture;
    static void init();
    static void setRenderTexture(Texture2D* texture);
    static void setViewport(int width, int height);
    [[nodiscard]] static int getViewportWidth();
    [[nodiscard]] static int getViewportHeight();
    static void drawCall();
    [[nodiscard]] static int getDrawCalls();
    static void clearDrawCalls();
};


#endif //POMEGRANATEENGINE_GRAPHICS_CORE_H
