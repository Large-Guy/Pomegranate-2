#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <component.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <model2d.h>
#include <matrix3x3.h>
#include <cmath>

int main() {

    GraphicsCore::init();

    Window window;
    window.setSize(1080, 720);
    window.setTitle("Pomegranate Engine");
    window.open();

    GraphicsCore::setViewport(1080, 720);

    Texture2D texture("assets/images/batman.png");
    Texture2D texture2("assets/images/pomegranate.png");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    float time = 0.0f;

    double lastFrameTime = glfwGetTime();

    std::cout << "Max texture image units: " << GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS << std::endl;

    while (window.isOpen()) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Game loop
        window.pollEvents();
        window.draw.begin();
        window.draw.setColor(Vector3(1.0,1.0,1.0));

        window.draw.clear();

        //Draw both textures
        window.draw.setZIndex(0.5f);
        window.draw.drawTexture(texture, Vector2(-128.0f, 0.0f), Vector2(256.0f, 256.0f), -time);
        window.draw.setZIndex(0.75f);
        window.draw.drawTexture(texture2, Vector2(128.0f, 0.0f), Vector2(256.0f, 256.0f), time);

        window.draw.end();
        time += 0.01f;

        std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
    }

    return 0;
}