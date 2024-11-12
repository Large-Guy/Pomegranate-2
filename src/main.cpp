#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/geometry/geometry.h>
#include "lua/lua_state.h"
#include "lua/debug.h"
#include "lua/events.h"
#include "lua/ecs.h"

int main() {

//#define GRAPHICS

#ifdef GRAPHICS
    Graphics::getInstance();

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();


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

    Mesh3D* mesh = new Mesh3D(vertices,indices, &shader);
//endregion


    double lastTime = glfwGetTime();
    double deltaTime = 0.0;

    Perspective perspective;

    //For now, we'll just use the identity matrix
    perspective.model = Matrix4x4();
    perspective.view = Matrix4x4();
    perspective.projection = Matrix4x4();


    while(window.isOpen()) {
        window.poll();

        shader._perspectiveSet.set(window._currentFrame,0, perspective);

        window.draw.begin();
        window.draw.clear({0.1, 0.1, 0.1, 1.0});

        window.draw.mesh(*mesh);

        window.draw.end();

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        //Debug::Log::info("FPS: ",1.0/deltaTime);
    }

    delete mesh;

    Debug::Log::info("------------------------------------------------------ Exiting... ------------------------------------------------------");

    return 0;
#else

    Extensions::Common::init();

    Entity parent = Entity::create();
    parent.add<Transform2D>(Vector2(0.0,0.0),Vector2(1.0,1.0),0.0f);
    parent.add<Name>("Parent");

    Entity child = Entity::create();
    child.add<Transform2D>(Vector2(0.0,0.0),Vector2(1.0,1.0),0.0f);
    child.add<Name>("Child");

    Entity child2 = Entity::create();
    child2.add<Transform2D>(Vector2(0.0,0.0),Vector2(1.0,1.0),0.0f);
    child2.add<Name>("Child2");

    Hierarchy::addChildTo(parent,child);
    Hierarchy::addChildTo(parent,child2);

    Debug::Log::info("Parent: ",parent.get<Name>()->name);
    Debug::Log::info("Child: ",child.get<Name>()->name);
    Debug::Log::info("Child2: ",child2.get<Name>()->name);

    SERIALIZE_TO_FILE(parent,"parent.bin");
    SERIALIZE_TO_FILE(child,"child1.bin");
    SERIALIZE_TO_FILE(child2,"child2.bin");




    return 0;

#endif
}