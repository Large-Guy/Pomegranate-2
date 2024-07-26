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

    Group group;

    //Populate group with 100 entities
    for(int i = -2; i < 2; i++)
    {
        Entity* entity = new Entity();
        entity->addComponent<Transform>(TRANSFORM,{Vector2(i * 64,0),Vector2(64,64),0});
        entity->addComponent<Velocity>(VELOCITY,{Vector2(0,0),0});
        group.addEntity(entity);
    }

    GraphicsCore::init();
    Window window;
    window.setSize(800,600);
    window.setTitle("Pomegranate Engine");
    window.open();

    Texture2D texture("assets/images/batman.png","Batman");

    auto start = std::chrono::high_resolution_clock::now();
    while(window.isOpen())
    {
        window.pollEvents();
        window.draw.begin();
        window.draw.clear();
        for(auto&e : group.getEntities())
        {
            if(e->hasComponent(TRANSFORM))
            {
                Transform* transform = e->getComponent<Transform>(TRANSFORM);
                if(transform != nullptr)
                    window.draw.drawTexture(texture,transform->position,transform->scale,transform->rotation);
            }
        }
        window.draw.end();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        start = end;
        std::cout << "FPS: " << 1.0/elapsed.count() << std::endl;
    }

    return 0;
}