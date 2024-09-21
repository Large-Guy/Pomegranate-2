#ifndef POMEGRANATE_ENGINE_ECS_COMPONENT_H
#define POMEGRANATE_ENGINE_ECS_COMPONENT_H
#include <iostream>
#include <cstring>
#include "ecs_typedefs.h"
#include <unordered_map>
#include <core/serializable.h>
#include "core.h"

struct ComponentList
{
    ComponentID component;
    void* elements;
    bool* occupied;
    size_t element_size;
    size_t capacity;
    size_t count;

    explicit ComponentList(ComponentID component, size_t component_size);
    void resize(size_t new_size);
    void* get(size_t i) const;
    void remove(size_t i);
    size_t add();
    bool has(size_t i) const;
};

struct Component
{
private:
public:
    static ComponentID create(const std::string& component,size_t size);
    template<typename T>
    static ComponentID create(const std::string& component)
    {
        ComponentID id = create(component,sizeof(T));
        ECS::component_ids[typeid(T).hash_code()] = id;
        return id;
    }
    static ComponentID getComponentID(const std::string& component);
    static std::string getComponentName(ComponentID component);
};

#endif //POMEGRANATE_ENGINE_COMPONENT_H
