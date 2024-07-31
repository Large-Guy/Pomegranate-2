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
        shader.set("NORMAL",pomegranate_n);
        shader.set("TIME",t);
        shader.set("USE_NORMAL_MAP",true);
        window.draw.drawTexture(pomegranate,Vector2(0   ,0),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(-256,0),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(-512,0),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(512 ,0),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(256 ,0),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(0   ,256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(-256,256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(-512,256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(512 ,256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(256 ,256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(0   ,-256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(-256,-256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(-512,-256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(512 ,-256),Vector2(256,256),t);
        window.draw.drawTexture(pomegranate,Vector2(256 ,-256),Vector2(256,256),t);
        window.draw.end();

        t += 0.01f;
    }


    return 0;
}