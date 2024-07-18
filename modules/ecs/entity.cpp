#include "entity.h"

#include <utility>

uint Entity::entityCount = 0;

Entity::Entity() {
    id = entityCount++;
    name = "Entity" + std::to_string(id);
}

Entity::Entity(std::string name) : name(std::move(name)) {
    id = entityCount++;
}

uint Entity::getId() const {
    return id;
}

std::string Entity::getName() const {
    return name;
}

uint Entity::getEntityCount() {
    return entityCount;
}

void Entity::serialize(Archive &a) const {
    a << id << name;
}

void Entity::deserialize(Archive &a) const {
    a >> &id >> &name;
}
