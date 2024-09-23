#include "component.h"

#include <utility>

ComponentList::ComponentList(ComponentID component, size_t component_size) {
    this->element_size = component_size;
    this->count = 0;
    this->capacity = 0;
    this->elements = nullptr;
    this->occupied = nullptr;
    this->component = component;
}

void ComponentList::resize(size_t new_size) {
    if(elements == nullptr)
    {
        elements = malloc(element_size * new_size);
        occupied = (bool*)malloc(sizeof(bool) * new_size);
        //Mark all slots as unoccupied
        for(size_t i = 0; i < new_size; i++)
        {
            occupied[i] = false;
        }
        return;
    }
    elements = realloc(elements,element_size*new_size);
    occupied = (bool*)realloc(occupied,sizeof(bool)*new_size);
    //Mark all new slots as unoccupied
    for(size_t i = count; i < new_size; i++)
    {
        occupied[i] = false;
    }
}

size_t ComponentList::add() {
    if(count + 1 >= capacity) {
        capacity = count + 1;
        resize(capacity);
        //Mark the new slot as occupied
        occupied[count] = true;
        return count++;
    }
    else {
        //Find empty slot
        for(size_t i = 0; i < capacity; i++) {
            if (!occupied[i]) {
                occupied[i] = true;
                count++;
                return i;
            }
        }
    }
    throw std::runtime_error("No empty slots found!");
}

void* ComponentList::get(size_t i) const {
    return (void*)((char*)elements + element_size * i);
}

void ComponentList::remove(size_t i) {
    if(i >= capacity || !occupied[i])
    {
        throw std::runtime_error("Index out of bounds!");
    }
    occupied[i] = false;
    count--;
    //0 the memory
    memset((char*)elements + element_size * i,0,element_size);
}

bool ComponentList::has(size_t i) const {
    return occupied[i];
}

ComponentID Component::create(const std::string& component, size_t size, ClassFunctions functions) {
    ComponentID id = ECS::component_sizes.size();
    ECS::component_sizes[id] = size;
    ECS::component_names[component] = id;
    ECS::functions[id] = std::move(functions);
    return id;
}

ComponentID Component::getComponentID(const std::string &component) {
    if(ECS::component_names.find(component) == ECS::component_names.end())
    {
        throw std::runtime_error("Component \"" + component + "\" not found!");
    }
    return ECS::component_names[component];
}

std::string Component::getComponentName(ComponentID component) {
    for(auto& pair : ECS::component_names)
    {
        if(pair.second == component)
        {
            return pair.first;
        }
    }
    throw std::runtime_error("Component with ID " + std::to_string(component) + " not found!");
}