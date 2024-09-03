#include <utility>
#include <core/core.h>
#include <graphics/vulkan/graphics.h>
#include <ecs/ecs.h>
#include <ecs/extensions/common/common.h>
#include <math/math.h>
#include <thread>
#include <omp.h>

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

int main() {
    Graphics::init();
    std::vector<char> vertex = readFile("assets/graphics/shaders/vertex.spv");
    std::vector<char> fragment = readFile("assets/graphics/shaders/fragment.spv");

    Shader defaultShader = Shader(vertex,fragment);

    PomegranateWindow window;
    window.setTitle("Pomegranate Engine - Vulkan");
    window.show();

    while(window.isOpen())
    {
        window.poll();
    }

    return 0;
}