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

void Entity::addComponent(component_id component) {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id]; //Get current record
    Archetype* new_archetype = eRecord.archetype->addComponent(component); //Find the new archetype of the entity with new component
    moveEntityArchetype(eRecord.archetype,eRecord.row,new_archetype,component); //Move the archetype
    eRecord = {new_archetype,new_archetype->_components[0].data.size() - 1}; //Set the record to the new archetype and the new row
}

void *Entity::getComponent(component_id component) const {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id];
    Archetype*& archetype = eRecord.archetype;
    std::unordered_map<archetype_id,ComponentLocation> archetypes = Archetype::_componentIndex[component];
    if(archetypes.count(archetype->_id) == 0) {
        return nullptr;
    }

    ComponentLocation& aRecord = archetypes[archetype->_id];
    return archetype->_components[aRecord.column].data[eRecord.row];
}

void Entity::moveEntityArchetype(Archetype *current, size_t row, Archetype *next, component_id component) {
    for(auto& c : next->_components)
    {
        c.data.emplace_back(nullptr); //Add new row
    }
    for(auto& c : current->_components)
    {
        void* data = c.data[row]; //Get data from current Archetype
        c.data.erase(c.data.begin() + row); // Remove data from current Archetype
        int column = Archetype::_componentIndex[(int)c.component][next->_id].column; //Find column that next archetype component data is in
        next->_components[column].data[row] = data; //Set that new rows data to data
    }
}
