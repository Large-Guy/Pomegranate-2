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

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while(end != std::string::npos) {
        parts.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    parts.push_back(str.substr(start, end));
    return parts;
}

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

    InputManager inputManager(&window);

    inputManager.addButtonAlias("exit", Keyboard::KEY_ESCAPE, Gamepad::BUTTON_START);
    inputManager.addButtonAlias("jump", Keyboard::KEY_SPACE, Keyboard::KEY_Z, Gamepad::BUTTON_BOTTOM);

    inputManager.addAxisAlias("rotateX",Axis(Keyboard::KEY_A,Keyboard::KEY_D));
    inputManager.addAxisAlias("rotateX",Axis(Keyboard::KEY_LEFT,Keyboard::KEY_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::AXIS_LEFT_X, true));
    inputManager.setAxisAliasDeadzone("rotateX",0.1f);

    inputManager.addAxisAlias("rotateY",Axis(Keyboard::KEY_W,Keyboard::KEY_S));
    inputManager.addAxisAlias("rotateY",Axis(Keyboard::KEY_UP,Keyboard::KEY_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::AXIS_LEFT_Y,true));
    inputManager.setAxisAliasDeadzone("rotateY",0.1f);

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

    File meshFile = File("assets/graphics/models/teapot.obj");
    meshFile.open();

    List<Vertex3D> vertices{};
    List<unsigned int> indices{};

    std::vector<std::string> lines = split(meshFile.readText(), "\n");

    for(auto& line : lines) {
        if(line.find("v ") != std::string::npos) {
            List<std::string> parts = split(line, " ");
            Vertex3D vertex = {{std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])},{0.0f,0.0f},{0.0f,0.0f,1.0f},{(float)rand()/(float)INT_MAX,(float)rand()/(float)INT_MAX,(float)rand()/(float)INT_MAX}};
            vertices.add(vertex);

        } else if(line.find("f ") != std::string::npos) {
            List<std::string> parts = split(line, " ");
            for(int i = 1; i < parts.size(); i++) {
                List<std::string> ind = split(parts[i], "/");
                unsigned int index = std::stoi(ind[0]) - 1;
                indices.add(index);
            }
        }
    }

    Mesh<Vertex3D, unsigned int> mesh(vertices, indices);

    Matrix4x4 model = Matrix4x4::identity();
    Matrix4x4 view = Matrix4x4::identity().translate({0.0f, 0.0f, -10.0f});
    Matrix4x4 projection = Matrix4x4::perspective(45.0f * (float)M_PI / 180.0f, 800.0f / 600.0f, 0.01f, 1000.0f);

    float spinX = 0.0f;
    float spinY = 0.0f;

    Vector3 rotation;

    while(window.isOpen()) {
        window.poll();
        inputManager.update();

        if(inputManager.getButtonAlias("exit") == BUTTON_PRESSED) {
            window.close();
        }

        if(inputManager.getButtonAlias("jump") == BUTTON_PRESSED) {
            spinY += 0.1f;
        }

        float leftStickX = inputManager.getAxisAlias("rotateX");
        float leftStickY = inputManager.getAxisAlias("rotateY");

        spinX += leftStickY * 0.01f;
        spinY += leftStickX * 0.01f;

        rotation.x += spinX;
        rotation.y += spinY;

        spinX *= 0.97f;
        spinY *= 0.97f;

        window.draw.begin();

        model = Matrix4x4::transform({0.0, -1.5f, 0.0f}, {1.0f, 1.0f, 1.0f},{0.0f,0.0f,0.0f}).rotate(rotation);

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

#endif
}