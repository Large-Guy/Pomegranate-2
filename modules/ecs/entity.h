#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<core/serializable.h>
#include"archetype.h"
#include "ecs_typedefs.h"
#include "component.h"
#include "core.h"

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
    static bool hasComponent(EntityID entity, const std::string& component);
    static void* getComponent(EntityID entity, ComponentID component);
    static void* getComponent(EntityID entity, const std::string& component);
    static void* addComponent(EntityID entity, ComponentID component);
    static void* addComponent(EntityID entity, const std::string& component);

    EntityID id;
    Entity();
    Entity(EntityID id);
    Entity(const Entity& entity);

    Entity& operator=(const Entity& entity);
    explicit operator EntityID() const;

    [[nodiscard]] bool hasComponent(ComponentID component) const;
    [[nodiscard]] bool hasComponent(const std::string& component) const;
    [[nodiscard]] void* getComponent(ComponentID component) const;
    [[nodiscard]] void* getComponent(const std::string& component) const;
    template<typename T>
    T* getComponent(ComponentID component)
    {
        return (T*) getComponent(component);
    }
    template<typename T>
    T* getComponent(const std::string& component)
    {
        return (T*) getComponent(component);
    }
    void* addComponent(ComponentID component) const;
    void* addComponent(const std::string& component) const;
    template<typename T, typename... Args>
    T* addComponent(ComponentID component, Args&&... args)
    {
        void* loc = addComponent(component);
        new(loc) T(std::forward<Args>(args)...);
        return (T*)loc;
    }
    template<typename T, typename... Args>
    T* addComponent(const std::string& component, Args&&... args)
    {
        void* loc = addComponent(component);
        new(loc) T(std::forward<Args>(args)...);
        return (T*)loc;
    }
    [[nodiscard]] Type getType() const;
};

#endif //POMEGRANATEENGINE_ENTITY_H
