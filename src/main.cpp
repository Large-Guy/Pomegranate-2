#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>

int main() {

    Graphics::enableValidationLayers = true;

    auto vertexFile = File("assets/graphics/shaders/shader.vert.spv");
    vertexFile.open();
    auto fragmentFile = File("assets/graphics/shaders/shader.frag.spv");
    fragmentFile.open();

    auto vertexFile1 = File("assets/graphics/shaders/shader1.vert.spv");
    vertexFile1.open();
    auto fragmentFile1 = File("assets/graphics/shaders/shader1.frag.spv");
    fragmentFile1.open();

    auto vertexShader = vertexFile.readBuffer();
    auto fragmentShader = fragmentFile.readBuffer();

    auto vertexShader1 = vertexFile1.readBuffer();
    auto fragmentShader1 = fragmentFile1.readBuffer();

    Shader shader(vertexShader, fragmentShader);
    Shader shader1(vertexShader1, fragmentShader1);

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    while(window.isOpen()) {
        window.poll();

        window.draw.begin();
        window.draw.clear({0.0, 0.0, 0.0, 1.0});

        window.recordCommandBuffer(&shader);
        //window.recordCommandBuffer(&shader1);

        window.draw.end();
    }

    return 0;
}