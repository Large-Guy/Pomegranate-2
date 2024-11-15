#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/opengl/graphics.h>
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

    Graphics::getInstance();

    Window window{};

    window.setTitle("Pomegranate Engine - OpenGL");
    window.setSize(800, 600);
    window.show();

    File vertexFile("assets/graphics/shaders/opengl/shader.vert");
    vertexFile.open();
    File fragmentFile("assets/graphics/shaders/opengl/shader.frag");
    fragmentFile.open();

    RenderInfo renderInfo = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_NONE,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED
    };

    Shader<Vertex3D> shader(vertexFile.readText().c_str(), fragmentFile.readText().c_str(), renderInfo);

    // Triangle vertices using Vertex structs
    List<Vertex3D> vertices = {
            {{0.0f,  0.5f, 0.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,0.0f}}, // top
            {{-0.5f, -0.5f, 0.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f}}, // bottom left
            {{0.5f, -0.5f, 0.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f,1.0f}}  // bottom right
    };

    List<unsigned int> indices = {
            0, 1, 2  // draw a triangle
    };

    Mesh<Vertex3D, unsigned int> mesh(vertices, indices);

    Matrix4x4 model = Matrix4x4::identity();
    Matrix4x4 view = Matrix4x4::identity().translate({0.0f, 0.0f, -5.0f});
    Matrix4x4 projection = Matrix4x4::perspective(45.0f * (float)M_PI / 180.0f, 800.0f / 600.0f, 0.01f, 1000.0f);


    while(window.isOpen()) {
        window.draw.begin();

        model = Matrix4x4::transform({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, (float)glfwGetTime()});

        window.draw.clear({0.1,0.1,0.1,1.0});

        window.draw.shader(&shader);
        shader.setUniform<Matrix4x4>("model", model);
        shader.setUniform<Matrix4x4>("view", view);
        shader.setUniform<Matrix4x4>("projection", projection);

        window.draw.mesh(&mesh);

        window.draw.end();
        window.poll();
    }

    return 0;

#endif
}