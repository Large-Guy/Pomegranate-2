#include "group.h"

std::unordered_map<std::string, Group*> Group::_groupIndex;

void Group::addEntity(Entity *entity) {
    _entities.push_back(entity);
}

void Group::removeEntity(Entity *entity) {
    _entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
}

Entity* Group::getEntity(entity_id id) {
    for (Entity* entity: _entities) {
        if (entity->_id == id) {
            return entity;
        }
    }
    return nullptr;
}

void Group::setName(const std::string &name) {
    _name = name;
    _groupIndex[name] = this;
}