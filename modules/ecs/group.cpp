#include "group.h"

Group::Group() {

}

Group::~Group() {

}

void Group::addEntity(Entity *entity) {
    this->_entities.emplace(entity->_id,entity);
}

void Group::removeEntity(entity_id id) {
    this->_entities.erase(id);
}

void Group::removeEntity(Entity *entity) {
    this->_entities.erase(entity->_id);
}

Entity *Group::getEntity(entity_id id) {
    return this->_entities[id];
}

bool Group::hasEntity(entity_id id) {
    return this->_entities.count(id);
}

bool Group::hasEntity(Entity *entity) {
    return this->_entities.count(entity->_id);
}

std::vector<Entity *> Group::getEntities() {
    std::vector<Entity*> entities;
    for(auto& e : this->_entities)
    {
        entities.emplace_back(e.second);
    }
    return entities;
}

std::vector<Entity*> Group::getEntities(const entity_type& type) {
    //Sort type
    entity_type type_copy = type;
    std::sort(type_copy.begin(), type_copy.end());
    //Find archetype
    std::vector<Entity*> entities;
    for(auto& e : this->_entities)
    {
        if(hasEntity(e.first)) {
            if (Entity::_entityIndex[e.first].archetype->_type == type) {
                entities.emplace_back(e.second);
            }
        }
    }
    return entities;
}