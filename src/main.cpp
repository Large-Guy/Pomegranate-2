#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>

Vector2 rotate(float angle)
{
    return {cosf(angle), sinf(angle)};
}

struct Position : Reflectable
{
    float x;
    float y;

    Position() {
        x = 0;
        y = 0;
        property("x",&x);
        property("y",&y);
    }

    void serialize(Archive& a) const override {
        a << x << y;
    }

    void deserialize(Archive& a) override {
        a >> x >> y;
    }
};

int main() {

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
            .renderMode = RenderMode::Fill,
            .cullMode = CullMode::Back,
    };

    Shader shader(vertexShader, fragmentShader, renderInfo);
//endregion

//region Model
//RGB Square model
    std::vector<Vertex2D> vertices = {
            {{0.5f, 0.5f}, {1.0,0.0},{0.0f, 1.0f, 0.0f}},
            {{0.5f, -0.5f}, {1.0,1.0},{0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f}, {0.0,1.0},{1.0f, 1.0f, 1.0f}},
            {{-0.5f, 0.5f}, {0.0,0.0},{1.0f, 0.0f, 0.0f}},
    };

    std::vector<uint16_t> indicies = {
            2, 1, 0, 0, 3, 2
    };

    //Buffer
    Buffer<Vertex2D> vertexBuffer(vertices, BufferType::VertexBuffer);
    Buffer<uint16_t> indexBuffer(indicies,BufferType::IndexBuffer);
//endregion

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    while(window.isOpen()) {
        window.poll();

        window.draw.begin();
        window.draw.clear({0.0, 0.0, 0.0, 1.0});

        window.draw.drawBuffers(&vertexBuffer,&indexBuffer,&shader);

        window.draw.end();
    }

    return 0;
#else
    const EventID function = Event::getEventId("function");

    Event::on(function,[&](float arg){
        Debug::Log::info("Hello, World!",arg);
    });

    Event::call(function,1.0f);
#endif
}