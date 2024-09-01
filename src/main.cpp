#include<iostream>
#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <ecs/extensions/common/common.h>
#include <math/math.h>

void printEntity(Entity e)
{
    Type type = e.getType();
    for (auto& component : type)
    {
        std::string name = Component::getComponentName(component);
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
            else if(c->getPropertyType(property) == typeid(Vector3).hash_code())
            {
                auto* v = c->get<Vector3>(property);
                std::cout << "(" << v->x << "," << v->y << "," << v->z << ")";
            }
            else if(c->getPropertyType(property) == typeid(float).hash_code())
            {
                auto* f = c->get<float>(property);
                std::cout << *f;
            }
            else if(c->getPropertyType(property) == typeid(String).hash_code())
            {
                auto* s = c->get<String>(property);
                std::cout << *s;
            }
            std::cout << "," << std::endl;
        }
        std::cout << "]" << std::endl;
    }
}

int main() {

    registerCommonComponents();

    Entity e = Entity::create();
    e.addComponent<Name>("Name", "Entity");

    std::cout << "Entity: " << e.id << std::endl;
    printEntity(e);

    return 0;
}