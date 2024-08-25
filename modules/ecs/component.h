#ifndef POMEGRANATEENGINE_COMPONENT_H
#define POMEGRANATEENGINE_COMPONENT_H
#include <iostream>
#include <cstring>
#include "ecs_typedefs.h"
#include <unordered_map>

struct ComponentList
{
    ComponentID component;
    void* elements;
    size_t element_size;
    size_t count;

    explicit ComponentList(ComponentID component, size_t component_size);
    void resize(size_t new_size);
    void* get(size_t i) const;
    void remove(size_t i);
    void* add();
};

#endif //POMEGRANATEENGINE_COMPONENT_H
