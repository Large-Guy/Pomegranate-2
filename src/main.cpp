#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <component.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <model2d.h>

int main() {

    GraphicsCore::init();

    Window window;
    window.setSize(800, 600);
    window.setTitle("Pomegranate Engine");
    window.open();

    //Create a shader program

    std::string vertexShaderSource = "";
    std::string fragmentShaderSource = "";

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

    std::vector<Vector2> vertices = {
            {-16.0f,-16.0f},
            {-16.0f,16.0f},
            {16.0f,-16.0f},
            {16.0f,16.0f}
    };

    Model2D model;
    for (auto& vertex : vertices) {
        model.addVertex(Vertex2D(vertex));
    }

    model.addIndex(0);
    model.addIndex(1);
    model.addIndex(2);

    model.addIndex(2);
    model.addIndex(1);
    model.addIndex(3);

    model.regenerateBuffers();

    //Serialize the model

    Archive archive;
    model.serialize(archive);
    archive.write_to_file("model3d.bin");

    while (window.isOpen()) {
        // Game loop
        window.pollEvents();
        window.draw.begin();
        window.draw.setColor(Vector3(1.0,1.0,1.0));
        window.draw.clear();

        shader.use();
        shader.set("SCREEN_RESOLUTION", Vector2(800, 600));

        shader.set("ENTITY_POSITION", Vector2(-64, 0));
        model.draw();

        shader.set("ENTITY_POSITION", Vector2(64, 0));
        model.draw();

        window.draw.end();
    }

    return 0;
}