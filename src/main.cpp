#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>

Vector2 rotate(float angle)
{
    return {cosf(angle), sinf(angle)};
}

int main() {

//#define GRAPHICS

#ifdef GRAPHICS

    Graphics::enableValidationLayers = true;


//region Shader
    auto vertexFile = File("assets/graphics/shaders/shader.vert.spv");
    vertexFile.open();
    auto fragmentFile = File("assets/graphics/shaders/shader.frag.spv");
    fragmentFile.open();

    auto vertexShader = vertexFile.readBuffer();
    auto fragmentShader = fragmentFile.readBuffer();

    Shader shader(vertexShader, fragmentShader, {.renderMode=RenderMode::Fill});
//endregion

//region Model
//RGB Triangle model
    float angle = 0;

    std::vector<Vertex2D> vertices = {{rotate(angle), {0.5, 0.5}, {1.0, 0.0, 0.0}},
                                      {rotate(angle + 2.0944f), {0.5, 0.5}, {0.0, 1.0, 0.0}},
                                      {rotate(angle + 2.0944f * 2.0f), {0.5, 0.5}, {0.0, 0.0, 1.0}}};

    //Buffer
    Buffer<Vertex2D> vertexBuffer(vertices, BufferType::VertexBuffer);
//endregion

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    while(window.isOpen()) {
        window.poll();

        vertices = {{rotate(angle), {0.5, 0.5}, {1.0, 0.0, 0.0}},
                    {rotate(angle + 2.0944f), {0.5, 0.5}, {0.0, 1.0, 0.0}},
                    {rotate(angle + 2.0944f * 2.0f), {0.5, 0.5}, {0.0, 0.0, 1.0}}};
        vertexBuffer.set(vertices);
        vertexBuffer.regenerate();
        angle += 0.05f;

        window.draw.begin();
        window.draw.clear({0.0, 0.0, 0.0, 1.0});

        window.drawBuffer(&vertexBuffer,&shader);

        window.draw.end();
    }

    return 0;
#else

#endif
}