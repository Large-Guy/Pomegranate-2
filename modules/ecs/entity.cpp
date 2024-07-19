#include "entity.h"

#include <utility>

uint Entity::_entityCount = 0;

Entity::Entity() {
    _id = _entityCount++;
    _name = "Entity" + std::to_string(_id);
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

uint Entity::getEntityCount() {
    return _entityCount;
}

void Entity::serialize(Archive &a) const {
    a << _id << _name;
}

void Entity::deserialize(Archive &a) {
    a >> &_id >> &_name;
}
