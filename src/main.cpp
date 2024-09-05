#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>
#include <thread>

int main() {

    ECS::setThreadCount(ECS::getMaxThreadCount()/2);

    Graphics::enableValidationLayers = true;

    const EventID UPDATE = Event::getEventId("update");
    const EventID DRAW = Event::getEventId("draw");

    Extensions::Common::init();

    for(int i = 0; i < 64; i++)
    {
        Entity entity = Entity::create();
        entity.addComponent<Transform2D>(Extensions::Common::TRANSFORM_2D);

    }

    const QuestionID GET_ENTITY_COUNT = Question::getQuestionId("get_entity_count");

    Question::answer(GET_ENTITY_COUNT,std::function<int()>([&](){
        int count = 0;

        ECS::parallelEach<Transform2D>("Transform2D", [&](Transform2D *transform, const Entity& entity) {
            count++;
        });

        return count;
    }));

    int entityCount = Question::ask<int,Resolver::Sum>(GET_ENTITY_COUNT);

    std::cout << "Entity Count: " << entityCount << std::endl;

    int count = 0;
    //Mutex lock

    ECS::each<Transform2D>("Transform2D", [&](Transform2D *transform, const Entity& entity) {
        count++;
    });

    std::cout << "Entity Count: " << count << std::endl;

    return 0;
}