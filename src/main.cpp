#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <ecs/extensions/rendering/rendering.h>
#include <graphics/opengl/graphics.h>
#include <input/input.h>
#include <math/geometry/geometry.h>
#include "lua/lua_state.h"
#include "lua/debug.h"
#include "lua/events.h"
#include "lua/ecs.h"

int main() {

#pragma region Events
    const EventID DRAW = Event::getEventId("DRAW");
    const EventID UPDATE = Event::getEventId("UPDATE");
#pragma endregion

#pragma region EcsExtensions
    Extensions::Common::init();
    Extensions::Rendering::init();
    ECS::setThreadCount(8);
#pragma endregion

#pragma region GraphicsInitialization
    Graphics::getInstance();

    Window window{};

    window.setTitle("Pomegranate Engine - OpenGL");
    window.setSize(800, 600);
    window.show();

    Mesh<Vertex3D, unsigned int> sphere = Mesh<Vertex3D,unsigned int>::sphere(0.5f, 8, 8);
#pragma endregion

#pragma region Shader
    File vertexFile("assets/graphics/shaders/opengl/shader.vert");
    vertexFile.open();
    File fragmentFile("assets/graphics/shaders/opengl/shader.frag");
    fragmentFile.open();

    RenderInfo renderInfo = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_BACK,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED,
            .depthMode = DEPTH_MODE_LESS
    };

    Shader<Vertex3D> shader(vertexFile.readText().c_str(), fragmentFile.readText().c_str(), renderInfo);
#pragma endregion

#pragma region InputManagement
    InputManager inputManager(&window);

    inputManager.addButtonAlias("exit", Keyboard::KEY_ESCAPE, Gamepad::BUTTON_START);

#pragma endregion

    Entity camera = Entity::create();
    camera.add<Transform3D>()->position = {0.0f,0.0f,-15.0f};
    camera.add<Camera3D>();
    Camera3D::setMain(camera);

    Event::on(DRAW, Function::create<void>(Extensions::Rendering::render3d));
    Event::on(UPDATE, Function::create<void,float>([](float deltaTime){
        InputManager* inputManager = Window::getCurrent()->getInputManager();

        if(inputManager->getButtonAlias("exit")) {
            Window::getCurrent()->close();
        }
    }));

    float deltaTime = 0.016f;
    float lastFrame = 0.0f;

    float spawnTime = 0.0f;

    while(window.isOpen()) {
        window.poll();
        inputManager.update();

        spawnTime += deltaTime;

        Event::emit(UPDATE, deltaTime);

        window.draw.begin();

        window.draw.clear({0.2,0.2,0.2,1.0});

        Event::emit(DRAW);

        window.draw.end();

        deltaTime = (float)glfwGetTime() - lastFrame;
        lastFrame = (float)glfwGetTime();

        Debug::Log::info("FPS: " + std::to_string(1.0f / deltaTime));
    }

    return 0;
}