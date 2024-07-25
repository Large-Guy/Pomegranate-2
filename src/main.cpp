#include<iostream>
#include <vector2.h>
#include <entity.h>
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

int main() {

#define TRANSFORM 0

    Entity e;
    e.addComponent<Transform>(TRANSFORM,{Vector2(0,0),Vector2(1,1),0});

    GraphicsCore::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(800,600);
    window.open();

    Texture2D texture("assets/images/batman.png","batman");

    while(window.isOpen())
    {
        //Update
        window.pollEvents();
        //Input
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window.close();
        }

        //Move the entity
        auto* t = e.getComponent<Transform>(TRANSFORM);
        Vector2 move = Vector2(0,0);
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_W) == GLFW_PRESS)
        {
            move.y += 1;
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_S) == GLFW_PRESS)
        {
            move.y -= 1;
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_A) == GLFW_PRESS)
        {
            move.x -= 1;
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_D) == GLFW_PRESS)
        {
            move.x += 1;
        }
        t->position = t->position + move * 6.0f;

        //Render
        window.draw.begin();
        window.draw.clear();
        window.draw.drawTexture(texture,t->position,t->scale * 64,t->rotation);
        window.draw.end();
    }


    return 0;
}