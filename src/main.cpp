#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <component.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <chrono>

int main() {

#define POSITION 0
#define VELOCITY 1
#define NAME 2

    Entity entity = Entity();
    entity.addComponent(NAME);
    entity.addComponent(POSITION);
    entity.addComponent(VELOCITY);

    std::cout << entity.hasComponent(NAME) << std::endl;
    std::cout << entity.hasComponent(POSITION) << std::endl;
    std::cout << entity.hasComponent(VELOCITY) << std::endl;

    return 0;
}