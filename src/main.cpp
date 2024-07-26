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

int main() {

#define TRANSFORM 0
#define VELOCITY 1

    Entity e;
    e.addComponent<Transform>(TRANSFORM);

    Component<Transform>* transform = e.getComponent<Transform>(TRANSFORM);
    transform->_data.position = Vector2(1.0f, 2.0f);

    e.addComponent<Velocity>(VELOCITY);

    Component<Transform>* transform2 = e.getComponent<Transform>(TRANSFORM);
    Component<Velocity>* velocity = e.getComponent<Velocity>(VELOCITY);
    std::cout << transform2->_data.position.x << " " << transform2->_data.position.y << std::endl;

    return 0;
}