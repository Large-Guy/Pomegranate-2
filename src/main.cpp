#include<iostream>
#include <utility>
#include <core/pomegranate.h>
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

    ComponentID NAME = Component::create<std::string>("NAME");

    registerCommonComponents();
    Entity player = Entity::create();
    player.addComponent<Name>("Name","Player Player Player Player Player");
    player.addComponent<Transform3D>("Transform3D")->position = {1,2,3};

    Entity camera = Entity::create();
    camera.addComponent<Name>("Name","Camera Camera Camera Camera Camera Camera");
    //camera.addComponent<Transform3D>("Transform3D")->position = {0,5,0};

    //Hierarchy::addChildTo(player,camera);

    auto* a = player.getComponent<Name>("Name");
    auto* b = camera.getComponent<Name>("Name");

    if(a == b)
    {
        std::cout << "A is in the same memory location as B" << std::endl;
    }

    if(a->data() == b->data())
    {
        std::cout << "A string data is in the same memory location as B" << std::endl;
    }

    std::cout << "----------Printing player!----------" << std::endl;
    printEntity(player);
    std::cout << "----------Printing camera!----------" << std::endl;
    printEntity(camera);


    return 0;
}