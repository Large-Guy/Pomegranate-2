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

    texture.bind(0);

    //Create a shader program
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    //Load from file
    std::ifstream vertexShaderFile("assets/shaders/test/vertex.glsl");
    std::ifstream fragmentShaderFile("assets/shaders/test/fragment.glsl");

    if (vertexShaderFile.is_open()) {
        std::string line;
        while (getline(vertexShaderFile, line)) {
            vertexShaderSource += line + "\n";
        }
        vertexShaderFile.close();
    }

    if (fragmentShaderFile.is_open()) {
        std::string line;
        while (getline(fragmentShaderFile, line)) {
            fragmentShaderSource += line + "\n";
        }
        fragmentShaderFile.close();
    }

    Shader shader(vertexShaderSource, fragmentShaderSource);

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

        window.draw.setShader(&shader);
        window.draw.drawTexture(texture, Vector2(0.0,0.0), Vector2(256.0,256.0),time);

        window.draw.end();
        time += 0.01f;

        //std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
    }

    return 0;
}