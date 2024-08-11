#ifndef POMEGRANATEENGINE_COMPONENT_H
#define POMEGRANATEENGINE_COMPONENT_H
#include <iostream>
#include <cstring>
#include <ecs_typedefs.h>
#include <unordered_map>

class Components {
    static component_id _componentCount;
    static std::unordered_map<std::string, component_id> _componentIndex;
    static std::unordered_map<component_id, std::string> _componentNames;
public:
    static component_id cRegister();
    static component_id cRegister(const std::string& name);
    static component_id get(const std::string& name);
    static std::string get(component_id id);
};

template <typename T> class Component {
public:
    T _data = T();
    Component(){
        _data = T();
    }
    Component<T>& operator=(const T& data) {
        this->_data = data;
        return *this;
    }
};

struct ComponentList
{
    component_id type = 0;
    void* data = nullptr;
    size_t componentSize = 0;
    size_t componentCount = 0;
    ComponentList(component_id type);
    void setComponentSize(size_t size);
    void resize(size_t new_size);
    void* add();
    void* get(size_t index);
    void remove(size_t index);
    bool hasSize() const;
};

#endif //POMEGRANATEENGINE_COMPONENT_H
