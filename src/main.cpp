#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>

int main() {

    Graphics::enableValidationLayers = true;


//region Shader
    auto vertexFile = File("assets/graphics/shaders/shader.vert.spv");
    vertexFile.open();
    auto fragmentFile = File("assets/graphics/shaders/shader.frag.spv");
    fragmentFile.open();

    auto vertexShader = vertexFile.readBuffer();
    auto fragmentShader = fragmentFile.readBuffer();

    Shader shader(vertexShader, fragmentShader);
//endregion

//region Model
    std::vector<Vertex2D> vertices = {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}
    };

    //Buffer
    Buffer<Vertex2D> vertexBuffer(vertices, BufferType::VertexBuffer);
//endregion

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;

    while(window.isOpen()) {
        window.poll();

        window.draw.begin();
        window.draw.clear({0.0, 0.0, 0.0, 1.0});

        window.drawBuffer(&vertexBuffer,&shader);

        window.draw.end();

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Debug::Log::info(String("FPS: ") + String(1.0f / (float)deltaTime));
    }

    return 0;
}