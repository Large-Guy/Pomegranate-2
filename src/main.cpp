#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/math.h>
#include <omp.h>

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
        //Mutex lock
        omp_lock_t lock;
        omp_init_lock(&lock);

        ECS::parallelEach("Transform2D", [&](void *transform, const Entity& entity) {
            omp_set_lock(&lock);
            count++;
            omp_unset_lock(&lock);
        });

        omp_destroy_lock(&lock);

        return count;
    }));

    int entityCount = Question::ask<int,Resolver::Sum>(GET_ENTITY_COUNT);

    std::cout << "Entity Count: " << entityCount << std::endl;

    int count = 0;
    //Mutex lock

    ECS::each("Transform2D", [&](void *transform, const Entity& entity) {
        count++;
    });

    std::cout << "Entity Count: " << count << std::endl;

    return 0;
}