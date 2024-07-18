#include "entity.h"

uint Entity::entityCount = 0;

Entity::Entity() {
    id = entityCount++;
    name = "Entity" + std::to_string(id);
}

int Entity::getId() const {
    return id;
}

int Entity::getEntityCount() {
    return entityCount;
}

void Entity::serialize(Archive &a) const {
    a << id << name;
}

void Entity::deserialize(Archive &a) const {
    a >> &id >> &name;
}
