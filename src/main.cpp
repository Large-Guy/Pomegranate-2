#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <group.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <event_manager.h>

struct Transform : public Serializable
{
    Vector2 position;
    Vector2 scale;
    float rotation;

    Transform& operator=(const Transform& other)
    {
        position = other.position;
        scale = other.scale;
        rotation = other.rotation;
        return *this;
    }

    void serialize(Archive& a) const override {
        a << position;
        a << scale;
        a << rotation;
    }

    void deserialize(Archive& a) override {
        a >> &position;
        a >> &scale;
        a >> &rotation;
    }
};

struct Velocity
{
    Vector2 linearVelocity;
    float angularVelocity;
};

struct Name
{
    const char* name;
};

void update(void* data)
{
    float dt = *(float*)data;
    std::cout << "Update function event called, dt: " << dt << std::endl;
}

int main() {

    //Component IDs
#define TRANSFORM 0
#define VELOCITY 1
#define NAME 2

    //Event IDs
    const event_id UPDATE = Event::getEventId("update");
    Event::on(UPDATE,[&](void* data){
        float dt = *(float*)data;
        std::cout << "Update lambda event called, dt: " << dt << std::endl;
    });

    Event::on("update",update);

    Graphics::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(800, 600);
    window.open();

    Texture2D pomegranate("assets/images/pomegranate.png","pomegranate");
    Texture2D pomegranate_n("assets/images/pomegranate_n.png","pomegranate_n");

    Shader shader("assets/shaders/default2d/vertex.glsl","assets/shaders/default2d/fragment.glsl");

    float deltaTime = 0;

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    while(window.isOpen())
    {
        window.pollEvents();

        //Update
        Event::call(UPDATE,&deltaTime);

        window.draw.begin();
        window.draw.clear();
        window.draw.setShader(&shader);
        pomegranate_n.bind(1);

        window.draw.end();

        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;
    }


    return 0;
}