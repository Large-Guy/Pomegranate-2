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

void testFunc() {
    Debug::Log::info("Hello World!");
}

int main() {

#pragma region Events
    const EventID DRAW = Event::create("@draw");
    const EventID UPDATE = Event::create("@update");
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

    auto& exit = inputManager.addButtonAlias("exit", Keyboard::KEY_ESCAPE, Gamepad::BUTTON_START);
    exit.onPressed(Function::create<void>([](){
        Debug::Log::info("Exiting...");
        Window::getCurrent()->close();
    }));

    auto& forward = inputManager.addAxisAlias("forward",
                                              Axis(Keyboard::KEY_W, Keyboard::KEY_S),
                                              Axis(Gamepad::AXIS_RIGHT_Y)
                                              );

    Event::on("@forward-changed", Function::create<void, float>([](float position){
        Camera3D::getMain().get<Transform3D>()->position += Transform3D::getForward(Camera3D::getMain()) * position;

    }));

#pragma endregion

    Entity e_camera = Entity::create();
    e_camera.add<Transform3D>()->position = {0.0f,0.0f,-15.0f};
    e_camera.add<Camera3D>();
    Camera3D::setMain(e_camera);

    Entity e_sphere = Entity::create();
    e_sphere.add<Transform3D>();
    auto* instance = e_sphere.add<MeshInstance>();
    instance->mesh = &sphere;
    instance->shader = &shader;

    Event::on(DRAW, Function::create<void>(Extensions::Rendering::render3d));
    Event::on(UPDATE, Function::create<void,float>([](float deltaTime){
        InputManager* inputManager = Window::getCurrent()->getInputManager();
        Debug::Log::info(1.0/deltaTime);
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
    }

    return 0;
}