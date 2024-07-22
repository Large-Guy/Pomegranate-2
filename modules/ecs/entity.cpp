#include "entity.h"

#include <utility>

uint Entity::_entityCount = 0;
std::unordered_map<EntityID,Archetype*> Entity::entities;

Entity::Entity() {
    _id = _entityCount++;
    _name = "Entity" + std::to_string(_id);
    Entity::entities[_id] = new Archetype();
}

Entity::Entity(std::string name) : _name(std::move(name)) {
    _id = _entityCount++;
}

uint Entity::getId() const {
    return _id;
}

std::string Entity::getName() const {
    return _name;
}

bool Entity::operator==(const Entity &e) const {
    return _id == e._id;
}

bool Entity::operator!=(const Entity &e) const {
    return _id != e._id;
}

void Entity::setName(const std::string &name) {
    _name = name;
}

bool Entity::hasComponent(ComponentID c) {
    Archetype* archetype = entities[_id];
    std::unordered_set<ArchetypeID>& archetype_set = Component::components[c];
    return archetype_set.count(archetype->getId()) != 0;
}

void Entity::addComponent(ComponentID c, std::any component) {
    Entity::entities[this->_id] = Entity::entities[this->_id]->addComponent(c);
}


void Entity::serialize(Archive &a) const {
    a << _id << _name;
}

void Entity::deserialize(Archive &a) {
    a >> &_id >> &_name;
}

uint Entity::getEntityCount() {
    return _entityCount;
}
