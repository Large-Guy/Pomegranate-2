#include "entity.h"

#include <utility>

std::unordered_map<entity_id, EntityRecord> Entity::_entityIndex;
entity_id Entity::_entityCount = 0;

Entity::Entity() {
    this->_id = Entity::_entityCount++;
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
    if(Archetype::_componentIndex.count(component) == 0)
        return false;
    std::unordered_map<archetype_id, ComponentLocation>& archetypes = Archetype::_componentIndex[component];
    return archetypes.count(eRecord.archetype->_id);
}

void Entity::moveEntityArchetype(Archetype *current, size_t row, Archetype *next, component_id component) {
    size_t new_row = next->_components[0].componentCount; //Get the new row
    for(auto& c : current->_components) //Get this archetype's components
    {
        void* data = (char*)c.data + c.componentSize * row; //Retrieve the data location in the component list
        void* new_loc = next->_components[Archetype::_componentIndex[c.type][next->_id].column].add(); //Get the location
        memcpy(new_loc,data,c.componentSize); //Copy the data to the new location
        current->_components[Archetype::_componentIndex[c.type][current->_id].column].remove(row); //Remove the data from the current column
    }
    Entity::_entityIndex[this->_id] = {next,new_row}; //Change the archetype
}
