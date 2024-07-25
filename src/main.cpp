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

    Entity entity;
    entity.addComponent(POSITION);

    void* data = entity.getComponent(POSITION);

    if(data == nullptr)
    {
        std::cout << "IT WORKED!!!" << std::endl;
    }

    return 0;
}