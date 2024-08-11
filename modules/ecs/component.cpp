#include "component.h"

component_id Components::_componentCount = 1;
std::unordered_map<std::string, component_id> Components::_componentIndex = {};
std::unordered_map<component_id, std::string> Components::_componentNames = {};

component_id Components::cRegister() {
    return _componentCount++;
}

component_id Components::cRegister(const std::string &name) {
    if(!_componentIndex.empty() && _componentIndex.count(name) > 0)
    {
        return _componentIndex[name];
    }
    _componentIndex[name] = _componentCount;
    _componentNames[_componentCount] = name;
    return _componentCount++;
}

component_id Components::get(const std::string &name) {
    if(!_componentIndex.empty() && _componentIndex.count(name) > 0)
        return _componentIndex[name];
    else
        throw std::runtime_error("Nonexistent component!");
}

std::string Components::get(component_id id) {
    if(!_componentIndex.empty() && _componentNames.count(id) == 0)
    {
        throw std::runtime_error("Nonexistent component!");
    }
    return _componentNames[id];
}

ComponentList::ComponentList(component_id type) {
    this->componentSize = 0;
    this->componentCount = 0;
    this->data = nullptr;
    this->type = type;
}

void ComponentList::setComponentSize(size_t size) {
    componentSize = size;
}

void ComponentList::resize(size_t new_size) {
    if(componentSize == 0)
    {
        throw std::runtime_error("Component size not set!");
    }
    if(componentCount == new_size)
    {
        return;
    }
    if(new_size == 0)
    {
        free(data);
        data = nullptr;
        componentCount = 0;
        return;
    }
    data = realloc(data, componentSize * new_size);
}

void* ComponentList::add() {
    if(componentSize == 0)
    {
        throw std::runtime_error("Component size not set!");
    }
    if(data == nullptr)
    {
        data = malloc(componentSize);
        componentCount = 1;
        return data;
    }
    else
    {
        resize(componentCount + 1);
        componentCount++;
        return (void*)((char*)data + componentSize * (componentCount - 1));
    }
}

void *ComponentList::get(size_t index) {
    if(componentSize == 0)
    {
        throw std::runtime_error("Component size not set!");
    }
    return (void*)((char*)data + componentSize * index);
}

void ComponentList::remove(size_t index) {
    if(componentSize == 0)
    {
        throw std::runtime_error("Component size not set!");
    }
    if(componentCount - 1 <= 0)
    {
        free(data);
        data = nullptr;
        componentCount = 0;
        return;
    }
    void* new_data = malloc(componentSize * (componentCount - 1));
    memcpy(new_data, data, componentSize * index);
    memcpy((char*)new_data + componentSize * index, (char*)data + componentSize * (index + 1), componentSize * (componentCount - index - 1));
    free(data);
    data = new_data;
    componentCount--;
}

bool ComponentList::hasSize() const {
    if(componentSize > 0) {
        return true;
    }
    return false;
}
