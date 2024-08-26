#include "group.h"

std::unordered_map<std::string, Group*> Group::groups;

Group::Group(const std::string& name) : name(name) {
    if(groups.find(name) != groups.end()) {
        throw std::runtime_error("Group with name " + name + " already exists!");
    }
    groups[name] = this;
    this->component = ECS::registerComponent(name, 0);
}

Group::~Group() {
    groups.erase(name);
}

void Group::addEntity(EntityID entity) {
    Entity::addComponent(entity, component);
    entities.insert(entity);
}

void Group::removeEntity(EntityID entity) {
    //TODO: Remove component from entity
    entities.erase(entity);
}

bool Group::hasEntity(EntityID entity) {
    return entities.find(entity) != entities.end();
}

const std::unordered_set<EntityID>& Group::getEntities() const {
    return entities;
}

const std::string& Group::getName() const {
    return name;
}