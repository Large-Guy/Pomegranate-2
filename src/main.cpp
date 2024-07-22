#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <component.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>

void testEntity(Entity* entity)
{
    if (entity->hasComponent(0)) {
        std::cout << "Has position" << std::endl;
    }
    if (entity->hasComponent(1)) {
        std::cout << "Has velocity" << std::endl;
    }
    if (entity->hasComponent(2)) {
        std::cout << "Has name" << std::endl;
    }
}

int main() {

#define POSITION 0
#define VELOCITY 1
#define NAME 2

    Entity entity;
    entity.addComponent(POSITION,0);
    entity.addComponent(VELOCITY,1);

    Entity entity2;
    entity2.addComponent(POSITION,0);
    entity2.addComponent(NAME,"Name");

    testEntity(&entity);
    testEntity(&entity2);

    return 0;
}