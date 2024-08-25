#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<core/serializable.h>
#include"archetype.h"
#include "ecs_typedefs.h"
#include "component.h"
#include "ecs.h"

struct EntityRecord
{
    Archetype* archetype;
    size_t row;

    EntityRecord(Archetype* archetype, size_t row);
};

class Entity
{
public:
    //Static
    static void moveEntityArchetype(EntityID entity, Archetype* new_archetype);
    static bool hasComponent(EntityID entity, ComponentID component);
    static void* getComponent(EntityID entity, ComponentID component);
    static void addComponent(EntityID entity, ComponentID component);

    EntityID id;
    Entity();
    explicit Entity(EntityID id);
    Entity(const Entity& entity);

    Entity& operator=(const Entity& entity);
    explicit operator EntityID() const;

    bool hasComponent(ComponentID component) const;
    void* getComponent(ComponentID component) const;
    void addComponent(ComponentID component);
};

#endif //POMEGRANATEENGINE_ENTITY_H
