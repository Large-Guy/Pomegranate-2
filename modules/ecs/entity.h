#ifndef POMEGRANATE_ENGINE_ECS_ENTITY_H
#define POMEGRANATE_ENGINE_ECS_ENTITY_H
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
    static Entity create();

    //Static
    static void moveEntityArchetype(EntityID entity, Archetype* new_archetype);
    static bool hasComponent(EntityID entity, ComponentID component);
    static bool hasComponent(EntityID entity, const std::string& component);
    static void* getComponent(EntityID entity, ComponentID component);
    static void* getComponent(EntityID entity, const std::string& component);
    static void* addComponent(EntityID entity, ComponentID component);
    static void* addComponent(EntityID entity, const std::string& component);
    static void removeComponent(EntityID entity, ComponentID component);
    static void removeComponent(EntityID entity, const std::string& component);

    EntityID id;
    Entity();
    Entity(EntityID id);
    Entity(const Entity& entity);
    Entity(const Entity* entity);

    Entity& operator=(const Entity& entity);
    Entity& operator=(const EntityID& entity);
    bool operator==(const Entity& entity) const;
    explicit operator EntityID() const;
    bool exists() const;
    [[nodiscard]] bool has(ComponentID component) const;
    [[nodiscard]] bool has(const std::string& component) const;

    template<typename T>
    [[nodiscard]] bool has() const
    {
        return has(ECS::component_ids[typeid(T).hash_code()]);
    }

    [[nodiscard]] void* get(ComponentID component) const;
    [[nodiscard]] void* get(const std::string& component) const;
    template<typename T>
    T* get()
    {
        return (T*) get(ECS::component_ids[typeid(T).hash_code()]);
    }
    template<typename T>
    T* get(ComponentID component)
    {
        return (T*) get(component);
    }
    template<typename T>
    T* get(const std::string& component)
    {
        return (T*) get(component);
    }
    void* add(ComponentID component) const;
    void* add(const std::string& component) const;
    template<typename T, typename... Args>
    T* addNamed(const std::string& component, Args&&... args)
    {
        Debug::AssertIf::isFalse(ECS::component_ids.count(typeid(T).hash_code()),"Component not registered!");
        T* t = (T*) add(component);
        return new (t) T(std::forward<Args>(args)...);
        return t;
    }
    template<typename T, typename... Args>
    T* add(Args&&... args)
    {
        Debug::AssertIf::isFalse(ECS::component_ids.count(typeid(T).hash_code()),"Component not registered!");
        T* t = (T*) add(ECS::component_ids[typeid(T).hash_code()]);
        return new (t) T(std::forward<Args>(args)...);
    }

    void remove(ComponentID component) const;
    void remove(const std::string& component) const;
    [[nodiscard]] Type getType() const;
};

template<>
struct std::hash<Entity>
{
    std::size_t operator()(const Entity& entity) const
    {
        return std::hash<EntityID>()(entity.id);
    }
};
#endif //POMEGRANATE_ENGINE_ENTITY_H
