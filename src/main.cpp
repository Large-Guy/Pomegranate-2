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
#include "audio/audio.h"

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

    Audio::getInstance();

    Stream stream;

    stream.setCustomCallback(Function::create<void,Stream::CallbackInfo>([](Stream::CallbackInfo info) {
        for (int i = 0; i < info.frameCount; i++) {
            float sample = sinf(2.0f * M_PI * 440.0f * (info.time + info.frameDeltaTime * (float)i));

            for (int j = 0; j < info.channels; j++) {
                info.output[i * info.channels + j] = sample;
            }
        }
    }));

    stream.start();

    while(true);

    return 0;

#endif
}