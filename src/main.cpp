#include<iostream>
#include<window.h>
#include<rect.h>
#include<group.h>
#include<event_manager.h>
#include<noise2d.h>


//Game
#include "systems/tile_map.h"


int main() {
    //Initailize Graphics

    Graphics::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(1080, 720);
    window.open();

    glfwSwapInterval(0);

    Texture2D* batman = new Texture2D("assets/images/batman.png");
    Texture2D* pomegranate = new Texture2D("assets/images/pomegranate.png");

    Shader* tileMapShader = new Shader("assets/shaders/default2d/vertex.glsl", "assets/shaders/default2d/fragment.glsl");

    //Define events

    const event_id EVENT_RENDER = Event::getEventId("render");
    const event_id EVENT_UPDATE = Event::getEventId("update");

    //Create groups

    Group world = Group("world");

    Group actors = Group("actors");

    //Create entities


    Entity* map = new Entity();

    TileSet* tileSet = map->addComponent<TileSet>(TILE_SET);
    tileSet->addTile(Tile{Rect(0,0,1,1), Vector2(0,0),batman});
    tileSet->addTile(Tile{Rect(0,0,1,1), Vector2(0,0),pomegranate});

    auto* transform = map->addComponent<Transform2D>(TRANSFORM_2D);
    transform->position = Vector2(0,0);
    transform->scale = Vector2(32);
    transform->rotation = 0;

    TileMap* tileMap = map->addComponent<TileMap>(TILE_MAP);
    tileMap->shader = tileMapShader;

    //Generate a basic world
    Noise2D noise = Noise2D();
    for (int y = 0; y < 1024; ++y) {
        for (int x = 0; x < 1024; ++x) {
            float n = noise.sample(Vector2(x,y)*0.1);
            if(n > 0.5) {
                tileMap->setTile(Vector2i(x,y),0);
            }
        }
    }

    world.addEntity(map);

    Event::on(EVENT_RENDER, tileMapRender);

    //Calculate delta time
    double lastTime = glfwGetTime();
    double deltaTime = 0.0;

    while(window.isOpen())
    {
        window.pollEvents();

        //Update
        Event::call(EVENT_UPDATE, nullptr);

        //Get mouse pos
        double posX, posY;
        glfwGetCursorPos(window.getGLFWwindow(),&posX,&posY);
        //Convert to pixel coordinates based on bottom left corner
        posY = (double)window.getHeight() - posY;

        auto* mapTransform = map->getComponent<Transform2D>(TRANSFORM_2D);

        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_W) == GLFW_PRESS) {
            mapTransform->position.y -= 5000 * deltaTime;
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_S) == GLFW_PRESS) {
            mapTransform->position.y += 5000 * deltaTime;
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_A) == GLFW_PRESS) {
            mapTransform->position.x += 5000 * deltaTime;
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_D) == GLFW_PRESS) {
            mapTransform->position.x -= 5000 * deltaTime;
        }

        std::cout << "Mouse Pos: " << posX << ", " << posY << std::endl;


        window.draw.begin();
        window.draw.setColor(Vector4{1.0,0.0,0.0,1.0});
        window.draw.clear();
        Event::call(EVENT_RENDER, nullptr);
        window.draw.end();

        //Calculate delta time
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::cout << "FPS: " << 1.0 / deltaTime << std::endl;
        std::cout << "Draw Calls: " << Graphics::getDrawCalls() << std::endl;
    }

    return 0;
}