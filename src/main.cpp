#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>

void updateEntity(Entity entity) {
    if(entity.has("Number")) {
        int* num = entity.get<int>("Number");
        (*num)++;
    }
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

    RenderInfo renderInfo = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_NONE,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED
    };

    Shader shader = Shader<Vertex3D>(vertexShader, fragmentShader, renderInfo);
//endregion

//region Model
//RGB triangle model
    List<Vertex3D> vertices = {
            {{-0.5,-0.5,0.5},{0.0,0.0},{0.0,0.0,1.0},{1.0,0.0,0.0}},
            {{0.5,-0.5,0.5},{1.0,0.0},{0.0,0.0,1.0},{0.0,1.0,0.0}},
            {{-0.5,0.5,0.5},{0.0,1.0},{0.0,0.0,1.0},{1.0,1.0,0.0}},
            {{0.5,0.5,0.5},{1.0,1.0},{0.0,0.0,1.0},{0.0,0.0,1.0}}
    };

    List<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3
    };

    Mesh3D mesh(vertices,indices, &shader);
//endregion

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;

    while(window.isOpen()) {
        window.poll();

        shader.updateUniformBuffer(window._currentFrame);

        window.draw.begin();
        window.draw.clear({0.1, 0.1, 0.1, 1.0});

        window.draw.mesh(mesh);

        window.draw.end();

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Debug::Log::info("FPS: ",1.0/deltaTime);
    }

    return 0;
#else

    ECS::setThreadCount(8);

    Extensions::Common::init();

    using namespace Extensions::Common;

    Entity entity = Entity::create();
    entity.add<Transform2D>(Vector2{1,2},Vector2{1,1},0);

    if(entity.has<Transform2D>()) {
        Debug::Log::pass("Entity has Transform2D component: ");
        Debug::Log::info("Position:","(", entity.get<Transform2D>()->position.x, ", ", entity.get<Transform2D>()->position.y, ")");
        Debug::Log::info("Scale:","(", entity.get<Transform2D>()->scale.x, ", ", entity.get<Transform2D>()->scale.y, ")");
        Debug::Log::info("Rotation:",entity.get<Transform2D>()->rotation);
    }

    Debug::Log::pass("Done!");
#endif
}