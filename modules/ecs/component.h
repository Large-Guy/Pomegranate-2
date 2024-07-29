#ifndef POMEGRANATEENGINE_COMPONENT_H
#define POMEGRANATEENGINE_COMPONENT_H
#include <iostream>
#include <cstring>
#include <ecs_typedefs.h>

template <typename T> class Component {
public:
    T _data;
    Component<T>& operator=(const T& data) {
        this->_data = data;
        return *this;
    }
};

struct ComponentList
{
    component_id type;
    void* data;
    size_t componentSize;
    size_t componentCount;
    ComponentList(component_id type);
    void setComponentSize(size_t size);
    void resize(size_t new_size);
    void* add();
    void* get(size_t index);
    void remove(size_t index);
    bool hasSize() const;
};

#endif //POMEGRANATEENGINE_COMPONENT_H
