#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <component.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <model3d.h>

int main() {

    GraphicsCore::init();

    Window window;
    window.setSize(800, 600);
    window.setTitle("Pomegranate Engine");
    window.open();

    //Create a shader program

    const char* vertexShaderSource = "#version 460 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    const char* fragmentShaderSource = "#version 460 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

    Shader shader = Shader(vertexShaderSource, fragmentShaderSource);

    std::vector<Vector3> vertices = {
            {0.0f,0.0f,0.0f},
            {0.0f,1.0f,0.0f},
            {1.0f,0.0f,0.0f}
    };

    Model3D model;
    for (auto& vertex : vertices) {
        model.addVertex(Vertex3D(vertex));
    }

    model.addIndex(0);
    model.addIndex(1);
    model.addIndex(2);

    model.regenerateBuffers();


    while (window.isOpen()) {
        // Game loop
        window.pollEvents();
        window.draw.begin();
        window.draw.setColor(Vector3(1.0,1.0,1.0));
        window.draw.clear();

        shader.use();
        model.draw();

        window.draw.end();
    }

    return 0;
}