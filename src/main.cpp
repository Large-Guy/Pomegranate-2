#include<iostream>
#include <utility>
#include <core/pomegranate.h>
#include <ecs/ecs.h>
#include <math/pmath.h>

struct Transform : public Component
{
    Vector2 position;
    Vector2 scale;
    float rotation;
    Transform(const Vector2& position, const Vector2& scale, float rotation)
    {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
        property<Vector2>("position", &this->position);
        property<Vector2>("scale", &this->scale);
        property<float>("rotation", &this->rotation);
    }
};

struct Name : public Component
{
    std::string name;
    Name(std::string name)
    {
        this->name = std::move(name);
        property<std::string>("name", &this->name);
    }
};

void printEntity(Entity e)
{
    Type type = e.getType();
    for (auto& component : type)
    {
        std::string name = ECS::getComponentName(component);
        std::cout << name << " : [" << std::endl;
        auto* c = (Component*)e.getComponent(component);
        std::vector<std::string> properties = c->properties();
        for (auto& property : properties)
        {
            std::cout << property << ": ";
            if(c->getPropertyType(property) == typeid(Vector2).hash_code())
            {
                auto* v = c->get<Vector2>(property);
                std::cout << "(" << v->x << "," << v->y << ")";
            }
            else if(c->getPropertyType(property) == typeid(float).hash_code())
            {
                auto* f = c->get<float>(property);
                std::cout << *f;
            }
            else if(c->getPropertyType(property) == typeid(std::string).hash_code())
            {
                auto* s = c->get<std::string>(property);
                std::cout << *s;
            }
            std::cout << "," << std::endl;
        }
        std::cout << "]" << std::endl;
    }
}

int main() {
    ECS::registerComponent<Name>("Name");
    ECS::registerComponent<Transform>("Transform");
    Entity e;
    e.addComponent<Name>("Name","Player");
    e.addComponent<Transform>("Transform",Vector2(0,0),Vector2(1,1),0);
    printEntity(e);


    return 0;
}