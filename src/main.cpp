#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/opengl/graphics.h>
#include <input/input.h>
#include <math/geometry/geometry.h>
#include "lua/lua_state.h"
#include "lua/debug.h"
#include "lua/events.h"
#include "lua/ecs.h"

int main() {

    Graphics::getInstance();

    Window window{};

    window.setTitle("Pomegranate Engine - OpenGL");
    window.setSize(800, 600);
    window.show();

    InputManager inputManager(&window);

    inputManager.addButtonAlias("exit", Keyboard::KEY_ESCAPE, Gamepad::BUTTON_START);

    inputManager.addAxisAlias("rotateX",Axis(Keyboard::KEY_LEFT,Keyboard::KEY_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::BUTTON_DPAD_LEFT,Gamepad::BUTTON_DPAD_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::AXIS_RIGHT_X, true));
    inputManager.setAxisAliasDeadzone("rotateX",0.1f);

    inputManager.addAxisAlias("rotateY",Axis(Keyboard::KEY_UP,Keyboard::KEY_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::BUTTON_DPAD_UP,Gamepad::BUTTON_DPAD_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::AXIS_RIGHT_Y,true));
    inputManager.setAxisAliasDeadzone("rotateY",0.1f);

    inputManager.addAxisAlias("moveRight",Axis(Keyboard::KEY_A,Keyboard::KEY_D));
    inputManager.addAxisAlias("moveRight",Axis(Gamepad::AXIS_LEFT_X, true));
    inputManager.setAxisAliasDeadzone("moveRight",0.1f);

    inputManager.addAxisAlias("moveForward",Axis(Keyboard::KEY_W,Keyboard::KEY_S));
    inputManager.addAxisAlias("moveForward",Axis(Gamepad::AXIS_LEFT_Y, true));
    inputManager.setAxisAliasDeadzone("moveForward",0.1f);

    inputManager.addAxisAlias("moveUp",Axis(Keyboard::KEY_SPACE,Keyboard::KEY_LEFT_SHIFT, true));
    inputManager.addAxisAlias("moveUp",Axis(Gamepad::BUTTON_TOP,Gamepad::BUTTON_BOTTOM, true));
    inputManager.setAxisAliasDeadzone("moveUp",0.1f);


    File vertexFile("assets/graphics/shaders/opengl/shader.vert");
    vertexFile.open();
    File fragmentFile("assets/graphics/shaders/opengl/shader.frag");
    fragmentFile.open();

    RenderInfo renderInfo = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_NONE,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED,
            .depthMode = DEPTH_MODE_LESS
    };

    Shader<Vertex3D> shader(vertexFile.readText().c_str(), fragmentFile.readText().c_str(), renderInfo);

    Mesh<Vertex3D, unsigned int> mesh = Mesh<Vertex3D, unsigned int>("assets/graphics/models/teapot.obj");

    Matrix4x4 model = Matrix4x4::identity();
    Matrix4x4 view = Matrix4x4::transform({0.0f, 0.0f, -5.0f}, {1.0f, 1.0f, 1.0f},{0.0f,0.0f,0.0f});
    Matrix4x4 projection = Matrix4x4::perspective(45.0f * (float)M_PI / 180.0f, 800.0f / 600.0f, 0.01f, 1000.0f);

    Vector3 rotation = {};
    Vector3 position = {0.0f,0.0f,-10.0f};

    while(window.isOpen()) {
        window.poll();
        inputManager.update();

        if(inputManager.getButtonAlias("exit") == BUTTON_PRESSED) {
            window.close();
        }

        float rotateX = inputManager.getAxisAlias("rotateX");
        float rotateY = inputManager.getAxisAlias("rotateY");

        rotation.x += rotateY * 0.03f;
        rotation.y += rotateX * 0.03f;

        view = Matrix4x4::identity().translate(position).rotate(rotation);

        Vector3 forward = view.forward();
        Vector3 right = view.right();
        Vector3 up = view.up();

        float moveForward = inputManager.getAxisAlias("moveForward");
        float moveRight = inputManager.getAxisAlias("moveRight");
        float moveUp = inputManager.getAxisAlias("moveUp");

        position += forward * moveForward * 0.1f;
        position += right * moveRight * 0.1f;
        position += up * moveUp * 0.1f;

        window.draw.begin();

        window.draw.clear({0.1,0.1,0.1,1.0});

        window.draw.shader(&shader);
        shader.setUniform<Matrix4x4>("model", model);
        shader.setUniform<Matrix4x4>("view", view);
        shader.setUniform<Matrix4x4>("projection", projection);
        shader.setUniform<float>("time", (float)glfwGetTime());

        window.draw.mesh(&mesh);

        window.draw.end();
    }

    return 0;
}