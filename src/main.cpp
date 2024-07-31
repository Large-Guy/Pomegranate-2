#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <group.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <chrono>

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

int main() {

#define TRANSFORM 0
#define VELOCITY 1
#define NAME 2

    Graphics::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(800, 600);
    window.open();

    Texture2D pomegranate("assets/images/pomegranate.png","pomegranate");
    Texture2D pomegranate_n("assets/images/pomegranate_n.png","pomegranate_n");

    Shader shader("assets/shaders/default2d/vertex.glsl","assets/shaders/default2d/fragment.glsl");

    float t = 0;

    while(window.isOpen())
    {
        window.pollEvents();

        window.draw.begin();
        window.draw.clear();
        window.draw.setShader(&shader);
        pomegranate_n.bind(1);

        window.draw.end();

        t += 0.01f;
    }


    return 0;
}