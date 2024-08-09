#include<iostream>
#include<window.h>
#include<rect.h>
#include<group.h>
#include<event_manager.h>


//Game
#include "systems/tile_map.h"


int main() {
    //Initailize Graphics

    Graphics::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(800, 600);
    window.open();

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
    transform->position = Vector2(-800,-600);
    transform->scale = Vector2(16);
    transform->rotation = 0;

    TileMap* tileMap = map->addComponent<TileMap>(TILE_MAP);
    tileMap->resize(128,128);
    for(int y = 0; y < 600; y++) {
        for(int x = 0; x < 800; x++) {
            tileMap->setTile(x, y, 1);
        }
    }
    tileMap->shader = tileMapShader;

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

        for(int y = -2; y < 2; y++) {
            for(int x = -2; x < 2; x++) {
                map->getComponent<TileMap>(TILE_MAP)->setTile((int)posX / 8 + x, (int)posY / 8 + y, 2);
            }
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
    }

    return 0;
}