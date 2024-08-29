#ifndef POMEGRANATEENGINE_COMPONENT_H
#define POMEGRANATEENGINE_COMPONENT_H
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

struct Component : public Serializable
{
private:
    std::unordered_map<std::string, std::pair<size_t,void*>> _data;
protected:
    template<typename T>
    void property(const std::string& name, void* data)
    {
        // Add the property to the _data
        _data[name] = std::pair<size_t,void*>(typeid(T).hash_code(),data);
    }
public:
    template<typename T>
    T* get(const std::string& name)
    {
        // Get the property from the _data
        return (T*)(_data[name].second);
    }
    size_t getPropertyType(const std::string& name);
    std::vector<std::string> properties();
};

#endif //POMEGRANATEENGINE_COMPONENT_H
