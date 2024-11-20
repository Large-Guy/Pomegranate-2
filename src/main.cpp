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

    Extensions::Common::init();
    Extensions::Rendering::init();

    Graphics::getInstance();

    Window window{};

    window.setTitle("Pomegranate Engine - OpenGL");
    window.setSize(800, 600);
    window.show();

#pragma region InputManagement
    InputManager inputManager(&window);

    inputManager.addButtonAlias("exit", Keyboard::KEY_ESCAPE, Gamepad::BUTTON_START);

    inputManager.addAxisAlias("rotateX",Axis(Keyboard::KEY_LEFT,Keyboard::KEY_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::BUTTON_DPAD_LEFT,Gamepad::BUTTON_DPAD_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::AXIS_RIGHT_X, true));
    //inputManager.addAxisAlias("rotateX", Axis(Mouse::AXIS_DELTA_X, 0.1f, true));
    inputManager.setAxisAliasDeadzone("rotateX",0.01f);

    inputManager.addAxisAlias("rotateY",Axis(Keyboard::KEY_UP,Keyboard::KEY_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::BUTTON_DPAD_UP,Gamepad::BUTTON_DPAD_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::AXIS_RIGHT_Y,true));
    //inputManager.addAxisAlias("rotateY", Axis(Mouse::AXIS_DELTA_Y, 0.1f, true));
    inputManager.setAxisAliasDeadzone("rotateY",0.01f);

    inputManager.addAxisAlias("moveRight",Axis(Keyboard::KEY_A,Keyboard::KEY_D));
    inputManager.addAxisAlias("moveRight",Axis(Gamepad::AXIS_LEFT_X, true));
    inputManager.setAxisAliasDeadzone("moveRight",0.1f);

    inputManager.addAxisAlias("moveForward",Axis(Keyboard::KEY_W,Keyboard::KEY_S));
    inputManager.addAxisAlias("moveForward",Axis(Gamepad::AXIS_LEFT_Y, true));
    inputManager.setAxisAliasDeadzone("moveForward",0.1f);

    inputManager.addAxisAlias("moveUp",Axis(Keyboard::KEY_SPACE,Keyboard::KEY_LEFT_SHIFT, true));
    inputManager.addAxisAlias("moveUp",Axis(Gamepad::BUTTON_TOP,Gamepad::BUTTON_BOTTOM, true));
    inputManager.setAxisAliasDeadzone("moveUp",0.1f);
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

    Mesh<Vertex3D, unsigned int> mesh = Mesh<Vertex3D,unsigned int>::sphere(1.0f, 32, 32);

    Entity camera = Entity::create();
    camera.add<Transform3D>();
    camera.add<Camera3D>();
    Camera3D::setMain(camera);

    Entity entity = Entity::create();
    entity.add<Transform3D>()->position = {2.5f,0.0,0.0};
    entity.add<MeshInstance>()->mesh = &mesh;
    entity.add<Material>()->shader = &shader;

    Transform3D* cameraTransform = camera.get<Transform3D>();

    while(window.isOpen()) {
        window.poll();
        inputManager.update();

        //inputManager.getMouse().setPosition({(float)window.getPosition().x,(float)window.getPosition().y});

        if(inputManager.getButtonAlias("exit") == BUTTON_PRESSED) {
            window.close();
        }

        float rotateX = inputManager.getAxisAlias("rotateX");
        float rotateY = inputManager.getAxisAlias("rotateY");

        cameraTransform->rotation.x += rotateY * 0.03f;
        cameraTransform->rotation.y += rotateX * 0.03f;

        Vector3 forward = cameraTransform->getLocalMatrix().forward();
        Vector3 right = cameraTransform->getLocalMatrix().right();
        Vector3 up = cameraTransform->getLocalMatrix().up();

        float moveForward = inputManager.getAxisAlias("moveForward");
        float moveRight = inputManager.getAxisAlias("moveRight");
        float moveUp = inputManager.getAxisAlias("moveUp");

        cameraTransform->position += forward * moveForward * 0.1f;
        cameraTransform->position += right * moveRight * 0.1f;
        cameraTransform->position += up * moveUp * 0.1f;

        window.draw.begin();

        window.draw.clear({0.2,0.2,0.2,1.0});

        Extensions::Rendering::render3d();

        window.draw.end();
    }

    return 0;
}