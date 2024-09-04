#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>
#include <thread>

int main() {
    List<char> vertexShader = File("assets/graphics/shaders/vertex.spv").open().readBuffer();
    List<char> fragmentShader = File("assets/graphics/shaders/fragment.spv").open().readBuffer();

    const EventID UPDATE = Event::getEventId("update");
    const EventID DRAW = Event::getEventId("draw");

    Window window{};
    window.setTitle("Pomegranate Engine - Vulkan Again");
    window.setSize(800, 600);
    window.show();

    Event::on(UPDATE,std::function<void()>([&](){
        //Poll window events and update the window
        window.poll();
    }));

    while(window.isOpen())
    {
        Event::call(UPDATE);
        Event::call(DRAW);
    }

    window.destroy();
    return 0;
}