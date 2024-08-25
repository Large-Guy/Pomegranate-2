#include<iostream>
#include <core/pomegranate.h>
#include <ecs/ecs.h>
#include <ecs/entity.h>
#include <math/pmath.h>
#include <graphics/glfw/graphics.h>
#include <events/events.h>

struct Transform : public Serializable {
    Vector2 position;
    float rotation;

    Transform() : position(Vector2(0,0)), rotation(0) {}
    Transform(Vector2 position, float rotation) : position(position), rotation(rotation) {}

    void serialize(Archive& a) const override {
        a << position << rotation;
    }

    void deserialize(Archive& a) override {
        a >> &position >> &rotation;
    }
};

int main() {
    ComponentID transform = ECS::registerComponent(sizeof(Transform));
    ComponentID children = ECS::registerComponent(sizeof(std::vector<Entity>));

    std::vector<Entity> entities = std::vector<Entity>();

    for(auto i : Range(16))
    {
        entities.emplace_back();
        Entity& entity1 = entities[i];
        entity1.addComponent(transform);
        void* loc1 = entity1.getComponent(transform);
        new(loc1) Transform(Vector2(0,0),i);
    }

    entities[0].addComponent(children);
    void* loc = entities[0].getComponent(children);
    new(loc) std::vector<Entity>();

    for(auto i : Range(1,16))
    {
        Entity& entity1 = entities[0];
        auto* child = (std::vector<Entity>*)entity1.getComponent(children);
        child->push_back(entities[i]);
    }

    for(auto i : Range(16))
    {
        Entity& entity = entities[i];
        auto* loc = (Transform*)entity.getComponent(transform);
        std::cout << loc->position.x << "," << loc->position.y << "   " << loc->rotation << std::endl;
    }

    return 0;
}