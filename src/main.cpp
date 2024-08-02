#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <group.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <event_manager.h>

#define TRANSFORM 0
#define VELOCITY 1
#define NAME 2
#define PLAYER 3
#define SPRITE 4

Window window;

struct Transform : public Serializable
{
    Vector2 position = Vector2(0,0);
    Vector2 scale = Vector2(1,1);
    float rotation = 0;

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

struct Velocity : public Serializable
{
    Vector2 linearVelocity = Vector2(0,0);
    float angularVelocity = 0;

    Vector2 linearAcceleration = Vector2(0,0);
    float angularAcceleration = 0;

    Velocity& operator=(const Velocity& other)
    {
        linearVelocity = other.linearVelocity;
        angularVelocity = other.angularVelocity;
        linearAcceleration = other.linearAcceleration;
        angularAcceleration = other.angularAcceleration;
        return *this;
    }

    void serialize(Archive& a) const override {
        a << linearVelocity;
        a << angularVelocity;
        a << linearAcceleration;
        a << angularAcceleration;
    }

    void deserialize(Archive& a) override {
        a >> &linearVelocity;
        a >> &angularVelocity;
        a >> &linearAcceleration;
        a >> &angularAcceleration;
    }
};

struct Name : public Serializable
{
    std::string name = "";

    Name()
    {
        name = "";
    }

    Name& operator=(const Name& other)
    {
        if(!other.name.empty())
            name = other.name;
        return *this;
    }

    void serialize(Archive& a) const override {
        a << name;
    }

    void deserialize(Archive& a) override {
        a >> &name;
    }
};

struct Player : public Serializable
{
    float speed = 0;
    float deceleration = 0;

    Player& operator=(const Player& other)
    {
        speed = other.speed;
        deceleration = other.deceleration;
        return *this;
    }

    void serialize(Archive& a) const override {
        a << speed;
        a << deceleration;
    }

    void deserialize(Archive& a) override {
        a >> &speed;
        a >> &deceleration;
    }
};

struct Sprite : public Serializable
{
    Texture2D* texture = nullptr;
    Texture2D* normalMap = nullptr;

    Sprite& operator=(const Sprite& other)
    {
        texture = other.texture;
        normalMap = other.normalMap;
        return *this;
    }

    void serialize(Archive& a) const override {
        a << texture;
    }

    void deserialize(Archive& a) override {
        a >> texture;
    }
};

void player_update(void* data)
{
    float dt = *(float*)data;

    Group* group = Group::getGroup("main");
    group->each({PLAYER},[&](Entity* e){
        auto* velocity = e->getComponent<Velocity>(VELOCITY);
        auto* player = e->getComponent<Player>(PLAYER);


        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_W) == GLFW_PRESS)
        {
            velocity->linearAcceleration = velocity->linearAcceleration + Vector2(0,player->speed);
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_S) == GLFW_PRESS)
        {
            velocity->linearAcceleration = velocity->linearAcceleration + Vector2(0,-player->speed);
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_A) == GLFW_PRESS)
        {
            velocity->linearAcceleration = velocity->linearAcceleration + Vector2(-player->speed,0);
        }
        if(glfwGetKey(window.getGLFWwindow(),GLFW_KEY_D) == GLFW_PRESS)
        {
            velocity->linearAcceleration = velocity->linearAcceleration + Vector2(player->speed,0);
        }


        velocity->linearAcceleration = velocity->linearAcceleration - velocity->linearVelocity * player->deceleration;
    });
}

void physics_update(void* data)
{
    float dt = *(float*)data;

    Group* group = Group::getGroup("main");
    group->each({TRANSFORM,VELOCITY},[&](Entity* e){
        auto* transform = e->getComponent<Transform>(TRANSFORM);
        auto* velocity = e->getComponent<Velocity>(VELOCITY);

        velocity->linearVelocity = velocity->linearVelocity + velocity->linearAcceleration * dt;
        velocity->angularVelocity = velocity->angularVelocity + velocity->angularAcceleration * dt;

        transform->position = transform->position + velocity->linearVelocity * dt;
        transform->rotation = transform->rotation + velocity->angularVelocity * dt;

        velocity->linearAcceleration = Vector2(0,0);
        velocity->angularAcceleration = 0;
    });
}

void sprite_draw(void* data)
{
    Group* group = Group::getGroup("main");
    group->each({TRANSFORM,SPRITE},[&](Entity* e){
        auto* transform = e->getComponent<Transform>(TRANSFORM);
        auto* sprite = e->getComponent<Sprite>(SPRITE);

        if(sprite->texture != nullptr) {
            sprite->normalMap->bind(1);
            window.draw.getShader()->set("NORMAL", sprite->normalMap);
            window.draw.getShader()->set("USE_NORMAL_MAP", 1);
            window.draw.drawTexture(sprite->texture, transform->position, transform->scale, transform->rotation);
        }
    });
}

int main() {

    Graphics::init();
    window.setTitle("Pomegranate Engine");
    window.setSize(800, 600);
    window.open();

    //Event IDs
    const event_id UPDATE = Event::getEventId("update");
    const event_id RENDER = Event::getEventId("render");
    Event::on(UPDATE,player_update);
    Event::on(UPDATE,physics_update);
    Event::on(RENDER,sprite_draw);

    //Main group
    Group mainGroup;
    mainGroup.setName("main");

    auto* texture = new Texture2D("assets/images/pomegranate.png","pomegranate");
    auto* texture_n = new Texture2D("assets/images/pomegranate_n.png","pomegranate_n");
    //Entity
    {
        Entity *entity = new Entity();
        entity->addComponent<Name>(NAME);
        entity->addComponent<Transform>(TRANSFORM)->scale = Vector2(128, 128);
        entity->addComponent<Velocity>(VELOCITY);
        auto* sprite = entity->addComponent<Sprite>(SPRITE);
        sprite->texture = texture;
        sprite->normalMap = texture_n;
        auto *player = entity->addComponent<Player>(PLAYER);
        player->speed = 10000;
        player->deceleration = 15;
        mainGroup.addEntity(entity);
    }

    //Delta time
    float deltaTime = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    float timePassed = 0;

    while(window.isOpen())
    {
        window.pollEvents();

        //Update
        Event::call(UPDATE,&deltaTime);


        //Render
        window.draw.begin();
        window.draw.setColor(Vector4(0.1f, 0.1f, 0.1f, 0.1f));
        window.draw.clear();
        window.draw.getShader()->set("TIME",timePassed);
        Event::call(RENDER,&deltaTime);

        window.draw.end();

        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;
        timePassed += deltaTime;
    }

    delete texture;
    return 0;
}