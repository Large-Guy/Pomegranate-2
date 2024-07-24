#include "entity.h"

#include <utility>

std::unordered_map<entity_id, Archetype*> Entity::_entityIndex;

Entity::Entity() {
    this->_id = Entity::_entityIndex.size();
    Archetype*& archetype = Archetype::_archetypeIndex[{}];
    if(archetype == nullptr)
    {
        archetype = new Archetype({});
    }
    Entity::_entityIndex[this->_id] = archetype;
}

Entity::~Entity() {
    delete Entity::_entityIndex[this->_id];
    Entity::_entityIndex.erase(this->_id);
}

bool Entity::hasComponent(component_id component) {
    Archetype*& archetype = Entity::_entityIndex[this->_id];
    std::unordered_set<archetype_id> archetype_set = Archetype::_componentIndex[component];
    return archetype_set.count(archetype->_id);
}

void Entity::addComponent(component_id component) {
    Archetype*& archetype = Entity::_entityIndex[this->_id];
    archetype = archetype->addComponent(component);
}
