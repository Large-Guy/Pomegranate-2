#include "component.h"

ComponentList::ComponentList(ComponentID component, size_t component_size) {
    this->element_size = component_size;
    this->count = 0;
    this->elements = nullptr;
    this->component = component;
}

void ComponentList::resize(size_t new_size) {
    if(elements == nullptr)
    {
        elements = malloc(element_size * new_size);
        return;
    }
    elements = realloc(elements,element_size*new_size);
}

void* ComponentList::add() {
    resize(++count);
    return (void*)((char*)elements + element_size * (count));
}

void* ComponentList::get(size_t i) const {
    return (void*)((char*)elements + element_size * i);
}

void ComponentList::remove(size_t i) {
    if(i >= count)
    {
        throw std::runtime_error("Index out of bounds");
    }
    if(i == count - 1)
    {
        count--;
        return;
    }
    void* dest = (void*)((char*)elements + element_size * i);
    void* src = (void*)((char*)elements + element_size * (count - 1));
    memmove(dest,src,element_size);
    count--;
}
