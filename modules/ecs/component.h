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

template<typename T>
void _constructor(void* ptr)
{
    new(ptr) T();
}

template<typename T>
void _destructor(void* ptr)
{
    ((T*)ptr)->~T();
}

template<typename T>
void _copy(void* dest, void* src)
{
    //Make sure to use the copy constructor because some objects are not trivially copyable
    T* d = (T*)dest;
    T* s = (T*)src;
    new(dest) T(*(T*)src);
}

template<typename T>
void _move(void* dest, void* src)
{
    //Make sure to use the move constructor because some objects are not trivially copyable
    new(dest) T(std::move(*(T*)src));
}

template<typename T>
void _serialize(Archive& archive, void* ptr)
{
    ((T *) ptr)->serialize(archive);
}

template<typename T>
void _deserialize(Archive& archive, void* ptr)
{
    ((T *) ptr)->deserialize(archive);
}


struct Component
{
private:
public:
    static ComponentID create(const std::string& component,size_t size, ClassFunctions functions);
    template<typename T>
    static ComponentID create(const std::string& component)
    {
        ComponentID id = create(component,sizeof(T), ClassFunctions{
                _constructor<T>,
                _destructor<T>,
                _copy<T>,
                _move<T>,
                _serialize<T>,
                _deserialize<T>
        });
        ECS::component_ids[typeid(T).hash_code()] = id;
        return id;
    }
    static ComponentID getComponentID(const std::string& component);
    static std::string getComponentName(ComponentID component);
};

#endif //POMEGRANATE_ENGINE_COMPONENT_H
