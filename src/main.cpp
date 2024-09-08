#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>
#include <omp.h>

int main() {

    Graphics::enableValidationLayers = true;

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    auto vertexFile = File("assets/graphics/shaders/shader.vert.spv");
    vertexFile.open();
    auto fragmentFile = File("assets/graphics/shaders/shader.frag.spv");
    fragmentFile.open();

    auto vertexShader = vertexFile.readBuffer();
    auto fragmentShader = fragmentFile.readBuffer();

    Shader shader(vertexShader, fragmentShader);

    Graphics::getInstance()->createRenderPass(&window);
    Graphics::getInstance()->createGraphicsPipeline(&window);

    while(window.isOpen()) {
        window.poll();
    }

    return 0;
}