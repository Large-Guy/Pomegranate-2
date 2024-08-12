#include<iostream>
#include<window.h>
#include<rect.h>
#include<group.h>
#include<event_manager.h>
#include<noise2d.h>


//Game
#include "systems/s_tile_map.h"
#include "systems/s_camera.h"

void moveCamera(float deltaTime) {
    Group* actors = Group::find("actors");

    actors->each({COMPONENT_CAMERA, COMPONENT_TRANSFORM_2D}, [&](Entity* entity) {
        Camera* camera = entity->getComponent<Camera>(COMPONENT_CAMERA);
        Transform2D* transform = entity->getComponent<Transform2D>(COMPONENT_TRANSFORM_2D);

        if(glfwGetKey(Window::current()->getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
        {
            transform->position.y -= 2048.0f * deltaTime;
        }
        if(glfwGetKey(Window::current()->getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
        {
            transform->position.y += 2048.0f * deltaTime;
        }
        if(glfwGetKey(Window::current()->getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
        {
            transform->position.x += 2048.0f * deltaTime;
        }
        if(glfwGetKey(Window::current()->getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
        {
            transform->position.x -= 2048.0f * deltaTime;
        }
        if(glfwGetKey(Window::current()->getGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            transform->scale.x += 1.0f * deltaTime;
            transform->scale.y += 1.0f * deltaTime;
        }
        if(glfwGetKey(Window::current()->getGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS)
        {
            transform->scale.x -= 1.0f * deltaTime;
            transform->scale.y -= 1.0f * deltaTime;
        }

        //Get mouse scroll for zoom

    });
}

int main() {
    //Initailize Graphics
    COMPONENT_TILE_MAP = Components::cRegister("TileMap");
    COMPONENT_TILE_SET = Components::cRegister("TileSet");
    COMPONENT_TRANSFORM_2D = Components::cRegister("Transform2D");
    COMPONENT_CAMERA = Components::cRegister("Camera");

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
    {
        auto *map = new Entity();

        auto *tileSet = map->addComponent<TileSet>(COMPONENT_TILE_SET);
        tileSet->addTile(Tile{Rect(0, 0, 1, 1), Vector2(0, 0), batman});
        tileSet->addTile(Tile{Rect(0, 0, 1, 1), Vector2(0, 0), pomegranate});

        auto *transform = map->addComponent<Transform2D>(COMPONENT_TRANSFORM_2D);
        transform->position = Vector2(0, 0);
        transform->scale = Vector2(32.0f);
        transform->rotation = 0;

        auto *tileMap = map->addComponent<TileMap>(COMPONENT_TILE_MAP);
        tileMap->shader = tileMapShader;

        //Generate a basic world
        Noise2D noise = Noise2D();
        for (int y = -0; y < 256; ++y) {
            for (int x = -0; x < 256; ++x) {
                float n = noise.sample(Vector2(x, y) * 0.1);
                if (n > 0.5) {
                    tileMap->setTile(Vector2i(x, y), 0);
                }
            }
        }

        world.addEntity(map);
    }
    {
        auto *camera = new Entity();

        auto *transform = camera->addComponent<Transform2D>(COMPONENT_TRANSFORM_2D);
        transform->position = Vector2(0, 0);
        transform->scale = Vector2(1.0f);
        transform->rotation = 0;

        auto *cameraComponent = camera->addComponent<Camera>(COMPONENT_CAMERA);
        cameraComponent->makeCurrent();

        actors.addEntity(camera);
    }

    Event::on(EVENT_UPDATE, EventFunction(std::function<void(float)>(moveCamera)));
    Event::on(EVENT_RENDER, EventFunction(std::function<void()>(tileMapRender)));
    Event::on(EVENT_RENDER, EventFunction(std::function<void()>(cameraRender)));

    //Calculate delta time
    double lastTime = glfwGetTime();
    double deltaTime = 0.0;

    while(window.isOpen())
    {
        //Update
        Event::call(EVENT_UPDATE, (float)deltaTime);

        window.draw.begin();
        window.draw.setColor(Vector4{1.0,0.0,0.0,1.0});
        window.draw.clear();
        Event::call(EVENT_RENDER, (float)deltaTime);
        window.draw.end();

        //Calculate delta time
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::cout << "FPS: " << 1.0 / deltaTime << std::endl;
        std::cout << "Draw Calls: " << Graphics::getDrawCalls() << std::endl;

        window.pollEvents();
    }

    return 0;
}