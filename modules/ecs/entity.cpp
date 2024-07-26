#include "entity.h"

#include <utility>

std::unordered_map<entity_id, EntityRecord> Entity::_entityIndex;

Entity::Entity() {
    this->_id = Entity::_entityIndex.size();
    Archetype*& archetype = Archetype::_archetypeIndex[{}];
    if(archetype == nullptr)
    {
        archetype = new Archetype({});
    }
    Entity::_entityIndex[this->_id] = {archetype,0};
}

Entity::~Entity() {
    Entity::_entityIndex.erase(this->_id);
}

bool Entity::hasComponent(component_id component) const {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id];
    std::unordered_map<archetype_id, ComponentLocation> archetypes = Archetype::_componentIndex[component];
    return archetypes.count(eRecord.archetype->_id);
}

void Entity::moveEntityArchetype(Archetype *current, size_t row, Archetype *next, component_id component) {
    for(auto& c : next->_components)
    {
        c.resize(c.componentCount + 1);
    }
    for(auto& c : current->_components)
    {
        void* data = (char*)c.data + c.componentSize * row;
        int column = Archetype::_componentIndex[(int)c.component][next->_id].column;
        Column& next_column = next->_components[column];
        void* new_data = (char*)next_column.data + next_column.componentSize * next_column.componentCount;
        memcpy(new_data,data,c.componentSize);
        c.remove(row);
    }
}
