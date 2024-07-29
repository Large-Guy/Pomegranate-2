#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <group.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <chrono>

struct Transform
{
    Vector2 position;
    Vector2 scale;
    float rotation;
};

struct Velocity
{
    Vector2 linearVelocity;
    float angularVelocity;
};

struct Name
{
    std::string name;
};

int main() {

#define TRANSFORM 0
#define VELOCITY 1
#define NAME 2
    Group group;

    Entity* e1 = new Entity();
    e1->addComponent<Name>(NAME,{"Hello World!"});
    e1->addComponent<Transform>(TRANSFORM);

    Entity* e2 = new Entity();
    e2->addComponent<Name>(NAME,{"This is Entity 2"});
    e2->addComponent<Transform>(TRANSFORM);

    std::vector<Entity*> entities = {e1,e2};

    for(auto e : entities) {
        std::cout << "Name" << std::endl;
        std::cout << e->getComponent<Name>(NAME)->name << std::endl;
        std::cout << "Transform" << std::endl;
        std::cout << (std::string)e->getComponent<Transform>(TRANSFORM)->position << std::endl;
        std::cout << (std::string)e->getComponent<Transform>(TRANSFORM)->scale << std::endl;
        std::cout << e->getComponent<Transform>(TRANSFORM)->rotation << std::endl;
    }
    return 0;
}