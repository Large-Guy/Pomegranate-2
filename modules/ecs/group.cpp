#include "group.h"

std::unordered_map<std::string, Group*> Group::_groupIndex;

Group::Group(const std::string &name) {
    _name = name;
    _groupIndex[name] = this;
}

Group::~Group() {
    _groupIndex.erase(_name);
}

void Group::addEntity(Entity *entity) {
    _entities.push_back(entity);
}

void Group::removeEntity(Entity *entity) {
    _entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
}