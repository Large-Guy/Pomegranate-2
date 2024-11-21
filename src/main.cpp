#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <ecs/extensions/rendering/rendering.h>
#include <graphics/opengl/graphics.h>
#include <input/input.h>
#include <math/geometry/geometry.h>
#include "lua/lua_state.h"
#include "lua/debug.h"
#include "lua/events.h"
#include "lua/ecs.h"

ComponentID PHYSICS_BODY_3D;
ComponentID PHYSICS_3D;
ComponentID LINK_3D;

class Physics3D;

enum PhysicsBodyType {
    DYNAMIC,
    STATIC,
    KINEMATIC
};

class PhysicsBody3D : public Reflectable {
public:
    Physics3D* world;
    Vector3 oldPosition;
    Vector3 acceleration;
    float mass;
    PhysicsBodyType type;

    PhysicsBody3D() {
        oldPosition = {0.0f,0.0f,0.0f};
        acceleration = {0.0f,0.0f,0.0f};
        mass = 1.0f;
        property("oldPosition", &oldPosition);
        property("acceleration", &acceleration);
        property("mass", &mass);
    }

    PhysicsBody3D(const Vector3& oldPosition, const Vector3& acceleration, float mass) {
        this->oldPosition = oldPosition;
        this->acceleration = acceleration;
        this->mass = mass;
        property("oldPosition", &oldPosition);
        property("acceleration", &acceleration);
        property("mass", &mass);
    }

    void applyForce(const Vector3& force) {
        acceleration += force / mass;
    }

    void serialize(Archive& a) const override {
        a << oldPosition << acceleration << mass;
    }

    void deserialize(Archive& a) override {
        a >> oldPosition >> acceleration >> mass;
    }
};

class Physics3D : Reflectable {
private:
    std::vector<Entity> entities;
public:
    std::unordered_map<Vector3i, std::unordered_set<Entity>> chunks;
    float chunkSize = 2.0f;
    Vector3 gravity;
    int steps = 1;

    Physics3D() {
        gravity = {0.0f,-9.81f,0.0f};
        chunks = {};
        property("gravity", &gravity);
    }

    void addRigidbody(Entity& entity) {
        if(!entity.has<PhysicsBody3D>()) return;
        entities.push_back(entity);
        entity.get<PhysicsBody3D>()->world = this;
    }

    void removeRigidbody(Entity& entity) {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

    std::vector<Entity>& getRigidbodies() {
        return entities;
    }

    void serialize(Archive& a) const override {
        a << gravity;
    }

    void deserialize(Archive& a) override {
        a >> gravity;
    }
};

struct Link3D : Reflectable {
    Entity to;
    float targetDistance;

    Link3D() {
        to = NULL_ENTITY;
        targetDistance = 0.0f;
        property("to", &to);
        property("targetDistance", &targetDistance);
    }

    Link3D(Entity to, float targetDistance) {
        this->to = to;
        this->targetDistance = targetDistance;
        property("to", &to);
        property("targetDistance", &targetDistance);
    }

    void serialize(Archive& a) const override {
        a << to << targetDistance;
    }

    void deserialize(Archive& a) override {
        a >> to >> targetDistance;
    }
};

void solveSphereCollisions(Entity& entity, const Entity& other, Transform3D* transform, Transform3D* otherTransform) {
    if (entity == other) return;

    Vector3 to_other = otherTransform->position - transform->position;
    float distance = to_other.length();

    float radiusA = 0.5f;
    float radiusB = 0.5f;

    if (distance < radiusA + radiusB) {
        Vector3 correction = to_other.normalize() * ((radiusA + radiusB) - distance);
        transform->position -= correction * 0.5f;
        otherTransform->position += correction * 0.5f;
    }
}

void physics3d(float dt) {
    ECS::each<PhysicsBody3D>(PHYSICS_BODY_3D, [&](PhysicsBody3D* rigidbody, Entity& entity) {
        auto *transform = entity.get<Transform3D>();

        float deltaTime = dt / rigidbody->world->steps;

        for(int i = 0; i < rigidbody->world->steps; i++) {

            //Gravity
            rigidbody->applyForce(rigidbody->world->gravity);

            //Test constraints

            Vector3 constraint = {0.0f, 0.0f, 0.0f};
            float radius = 25.0f;

            Vector3 to_object = transform->position - constraint;
            float distance = to_object.length();

            if (distance > radius) {
                Vector3 correction = to_object.normalize() * (distance - radius);
                transform->position -= correction;
            }

            //Test collision

            std::array<Vector3i, 27> chunks = {};

            //Get all chunks around the entity
            Vector3i chunk = {int(transform->position.x / rigidbody->world->chunkSize), int(transform->position.y / rigidbody->world->chunkSize), int(transform->position.z / rigidbody->world->chunkSize)};
            chunks[0] = chunk;
            chunks[1] = {chunk.x + 1, chunk.y, chunk.z};
            chunks[2] = {chunk.x - 1, chunk.y, chunk.z};
            chunks[3] = {chunk.x, chunk.y + 1, chunk.z};
            chunks[4] = {chunk.x, chunk.y - 1, chunk.z};
            chunks[5] = {chunk.x, chunk.y, chunk.z + 1};
            chunks[6] = {chunk.x, chunk.y, chunk.z - 1};
            chunks[7] = {chunk.x + 1, chunk.y + 1, chunk.z};
            chunks[8] = {chunk.x - 1, chunk.y - 1, chunk.z};
            chunks[9] = {chunk.x + 1, chunk.y, chunk.z + 1};
            chunks[10] = {chunk.x - 1, chunk.y, chunk.z - 1};
            chunks[11] = {chunk.x, chunk.y + 1, chunk.z + 1};
            chunks[12] = {chunk.x, chunk.y - 1, chunk.z - 1};
            chunks[13] = {chunk.x + 1, chunk.y + 1, chunk.z + 1};
            chunks[14] = {chunk.x - 1, chunk.y - 1, chunk.z - 1};
            chunks[15] = {chunk.x + 1, chunk.y, chunk.z - 1};
            chunks[16] = {chunk.x - 1, chunk.y, chunk.z + 1};
            chunks[17] = {chunk.x, chunk.y + 1, chunk.z - 1};
            chunks[18] = {chunk.x, chunk.y - 1, chunk.z + 1};
            chunks[19] = {chunk.x + 1, chunk.y + 1, chunk.z - 1};
            chunks[20] = {chunk.x - 1, chunk.y - 1, chunk.z + 1};
            chunks[21] = {chunk.x + 1, chunk.y, chunk.z + 1};
            chunks[22] = {chunk.x - 1, chunk.y, chunk.z - 1};
            chunks[23] = {chunk.x, chunk.y + 1, chunk.z + 1};
            chunks[24] = {chunk.x, chunk.y - 1, chunk.z - 1};
            chunks[25] = {chunk.x + 1, chunk.y + 1, chunk.z + 1};
            chunks[26] = {chunk.x - 1, chunk.y - 1, chunk.z - 1};

            for(auto& chunk : chunks) {
                if(rigidbody->world->chunks.find(chunk) == rigidbody->world->chunks.end()) {
                    rigidbody->world->chunks[chunk] = {};
                }
            }

            for(auto& chunk : chunks) {
                for (auto &otherEntity: rigidbody->world->chunks[chunk]) {
                    auto *otherTransform = otherEntity.get<Transform3D>();

                    solveSphereCollisions(entity, otherEntity, transform, otherTransform);
                }
            }

            //Verlet Integration
            //Remove rigidbody from world chunk
            Vector3i oldChunk = {int(rigidbody->oldPosition.x / rigidbody->world->chunkSize), int(rigidbody->oldPosition.y / rigidbody->world->chunkSize), int(rigidbody->oldPosition.z / rigidbody->world->chunkSize)};
            if(rigidbody->world->chunks.find(oldChunk) == rigidbody->world->chunks.end()) {
                rigidbody->world->chunks[oldChunk] = {};
            }
            if(rigidbody->world->chunks[oldChunk].find(entity) != rigidbody->world->chunks[oldChunk].end())
                rigidbody->world->chunks[oldChunk].erase(entity);

            Vector3 velocity = transform->position - rigidbody->oldPosition;

            rigidbody->oldPosition = transform->position;

            transform->position = transform->position + velocity + rigidbody->acceleration * deltaTime * deltaTime;

            rigidbody->acceleration = {0.0f, 0.0f, 0.0f};

            //Add rigidbody to new world chunk
            Vector3i newChunk = {int(transform->position.x / rigidbody->world->chunkSize), int(transform->position.y / rigidbody->world->chunkSize), int(transform->position.z / rigidbody->world->chunkSize)};
            if(rigidbody->world->chunks.find(newChunk) == rigidbody->world->chunks.end()) {
                rigidbody->world->chunks[newChunk] = {};
            }
            rigidbody->world->chunks[newChunk].insert(entity);
        }
    });
}

int main() {

#pragma region Events
    const EventID DRAW = Event::getEventId("DRAW");
    const EventID UPDATE = Event::getEventId("UPDATE");
#pragma endregion

#pragma region EcsExtensions
    Extensions::Common::init();
    Extensions::Rendering::init();
    PHYSICS_BODY_3D = Component::create<PhysicsBody3D>("Rigidbody3D");
    PHYSICS_3D = Component::create<Physics3D>("Physics3D");
    LINK_3D = Component::create<Link3D>("Link");
    ECS::setThreadCount(8);
#pragma endregion

#pragma region GraphicsInitialization
    Graphics::getInstance();

    Window window{};

    window.setTitle("Pomegranate Engine - OpenGL");
    window.setSize(800, 600);
    window.show();

    Mesh<Vertex3D, unsigned int> sphere = Mesh<Vertex3D,unsigned int>::sphere(0.5f, 8, 8);
#pragma endregion

#pragma region Shader
    File vertexFile("assets/graphics/shaders/opengl/shader.vert");
    vertexFile.open();
    File fragmentFile("assets/graphics/shaders/opengl/shader.frag");
    fragmentFile.open();

    RenderInfo renderInfo = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_BACK,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED,
            .depthMode = DEPTH_MODE_LESS
    };

    Shader<Vertex3D> shader(vertexFile.readText().c_str(), fragmentFile.readText().c_str(), renderInfo);
#pragma endregion

#pragma region InputManagement
    InputManager inputManager(&window);

    inputManager.addButtonAlias("exit", Keyboard::KEY_ESCAPE, Gamepad::BUTTON_START);

    inputManager.addAxisAlias("rotateX",Axis(Keyboard::KEY_LEFT,Keyboard::KEY_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::BUTTON_DPAD_LEFT,Gamepad::BUTTON_DPAD_RIGHT));
    inputManager.addAxisAlias("rotateX",Axis(Gamepad::AXIS_RIGHT_X, true));
    //inputManager.addAxisAlias("rotateX", Axis(Mouse::AXIS_DELTA_X, 0.1f, true));
    inputManager.setAxisAliasDeadzone("rotateX",0.01f);

    inputManager.addAxisAlias("rotateY",Axis(Keyboard::KEY_UP,Keyboard::KEY_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::BUTTON_DPAD_UP,Gamepad::BUTTON_DPAD_DOWN));
    inputManager.addAxisAlias("rotateY",Axis(Gamepad::AXIS_RIGHT_Y,true));
    //inputManager.addAxisAlias("rotateY", Axis(Mouse::AXIS_DELTA_Y, 0.1f, true));
    inputManager.setAxisAliasDeadzone("rotateY",0.01f);

    inputManager.addAxisAlias("moveRight",Axis(Keyboard::KEY_A,Keyboard::KEY_D));
    inputManager.addAxisAlias("moveRight",Axis(Gamepad::AXIS_LEFT_X, true));
    inputManager.setAxisAliasDeadzone("moveRight",0.1f);

    inputManager.addAxisAlias("moveForward",Axis(Keyboard::KEY_W,Keyboard::KEY_S));
    inputManager.addAxisAlias("moveForward",Axis(Gamepad::AXIS_LEFT_Y, true));
    inputManager.setAxisAliasDeadzone("moveForward",0.1f);

    inputManager.addAxisAlias("moveUp",Axis(Keyboard::KEY_SPACE,Keyboard::KEY_LEFT_SHIFT, true));
    inputManager.addAxisAlias("moveUp",Axis(Gamepad::BUTTON_TOP,Gamepad::BUTTON_BOTTOM, true));
    inputManager.setAxisAliasDeadzone("moveUp",0.1f);

    inputManager.addButtonAlias("interact", Keyboard::KEY_E, Gamepad::BUTTON_RIGHT);
#pragma endregion

    Entity camera = Entity::create();
    camera.add<Transform3D>()->position = {0.0f,0.0f,-15.0f};
    camera.add<Camera3D>();
    Camera3D::setMain(camera);

    Entity physicsWorld = Entity::create();
    physicsWorld.add<Physics3D>();

    Event::on(DRAW, Function::create<void>(Extensions::Rendering::render3d));
    Event::on(UPDATE, Function::create<void,float>([](float deltaTime){
        InputManager* inputManager = Window::getCurrent()->getInputManager();
        auto* cameraTransform = Camera3D::getMain().get<Transform3D>();

        if(inputManager->getButtonAlias("exit") == BUTTON_PRESSED) {
            Window::getCurrent()->close();
        }

        float rotateX = inputManager->getAxisAlias("rotateX");
        float rotateY = inputManager->getAxisAlias("rotateY");

        cameraTransform->rotation.x += rotateY * 0.03f;
        cameraTransform->rotation.y += rotateX * 0.03f;

        Vector3 forward = cameraTransform->getLocalMatrix().forward();
        Vector3 right = cameraTransform->getLocalMatrix().right();
        Vector3 up = cameraTransform->getLocalMatrix().up();

        float moveForward = inputManager->getAxisAlias("moveForward");
        float moveRight = inputManager->getAxisAlias("moveRight");
        float moveUp = inputManager->getAxisAlias("moveUp");

        cameraTransform->position += forward * moveForward * 0.1f;
        cameraTransform->position += right * moveRight * 0.1f;
        cameraTransform->position += up * moveUp * 0.1f;
    }));

    Event::on(UPDATE, Function::create<void,float>(physics3d));

    float deltaTime = 0.016f;
    float lastFrame = 0.0f;

    float spawnTime = 0.0f;

    while(window.isOpen()) {
        window.poll();
        inputManager.update();

        spawnTime += deltaTime;

        if(inputManager.getButtonAlias("interact") && spawnTime > 0.02f) {
            spawnTime = 0.0f;
            Entity entity = Entity::create();
            entity.add<Transform3D>()->position = {0.0};
            entity.add<MeshInstance>()->mesh = &sphere;
            entity.add<Material>()->shader = &shader;

            float x = (float)rand()/(float)(INT_MAX) - 0.5f;
            float y = (float)rand()/(float)(INT_MAX) - 0.5f;
            float z = (float)rand()/(float)(INT_MAX) - 0.5f;

            entity.add<PhysicsBody3D>()->applyForce(Vector3(x, y, z) * 10.0f);

            physicsWorld.get<Physics3D>()->addRigidbody(entity);
        }

        Event::emit(UPDATE, deltaTime);

        window.draw.begin();

        window.draw.clear({0.2,0.2,0.2,1.0});

        Event::emit(DRAW);

        window.draw.end();

        deltaTime = (float)glfwGetTime() - lastFrame;
        lastFrame = (float)glfwGetTime();

        Debug::Log::info("FPS: " + std::to_string(1.0f / deltaTime));
    }

    return 0;
}