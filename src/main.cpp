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

    //Create a shader program

    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    //Load from file
    std::ifstream vertexShaderFile("assets/shaders/default2d/vertex.glsl");
    std::ifstream fragmentShaderFile("assets/shaders/default2d/fragment.glsl");

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

    Shader shader = Shader(vertexShaderSource, fragmentShaderSource);


    Model2D model;

    model.addVertex({
                            {-0.5,-0.5},
                            {1.0},
                            {0.0,0.0}
                    });
    model.addVertex({
                            {-0.5,0.5},
                            {1.0},
                            {0.0,1.0}
    });
    model.addVertex({
                            {0.5,0.5},
                            {1.0},
                            {1.0,1.0}
    });
    model.addVertex({
                            {0.5,-0.5},
                            {1.0},
                            {1.0,0.0}
    });

    model.addIndex(0);
    model.addIndex(1);
    model.addIndex(2);

    model.addIndex(0);
    model.addIndex(2);
    model.addIndex(3);

    model.regenerateBuffers();

    //Serialize the model

    Archive archive;
    model.serialize(archive);
    archive.writeToFile("model3d.bin");

    Texture2D texture("assets/images/batman.png");

    texture.bind(0);

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

        window.draw.drawTexture(&texture, Vector2(0.0,0.0), Vector2(128.0,128.0));
        window.draw.drawTexture(&texture, Vector2(-256.0,0.0), Vector2(128.0,128.0));
        window.draw.drawTexture(&texture, Vector2(256.0,0.0), Vector2(128.0,128.0));

        window.draw.end();
        time += 0.01f;

        //std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
    }

    return 0;
}