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

    const QuestionID TEST = Question::getQuestionId("test");
    Question::answer(TEST,std::function<bool()>([](){
        return false;
    }));

    Question::answer(TEST,std::function<bool()>([](){
        return true;
    }));

    auto result = Question::ask<bool,Resolver::Or>(TEST);

    std::cout << result << std::endl; //True



    while(window.isOpen())
    {
        Event::call(UPDATE);
        Event::call(DRAW);
    }
    return 0;
}