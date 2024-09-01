#include "entity.h"

#include <utility>

Entity Entity::create() {
    Entity entity;
    entity.id = ECS::entity_index.size() + 1;
    ECS::entity_index[entity.id] = new EntityRecord{Archetype::getArchetype({}),0};
    return entity;
}

EntityRecord::EntityRecord(Archetype *archetype, size_t row) {
    this->archetype = archetype;
    this->row = row;
}

void Entity::moveEntityArchetype(EntityID entity, Archetype *new_archetype) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* old_archetype = record->archetype;
    if(old_archetype == nullptr)
    {
        throw std::runtime_error("Somethings gone wrong. Archetype isn't supposed to be null!");
    }
    if(old_archetype == new_archetype)
    {
        return;
    }
    int row = 0;
    for(auto& list : new_archetype->components)
    {
        row = list.add();
    }
    new_archetype->entities[row] = entity;
    for(auto& list : record->archetype->components)
    {
        if(new_archetype->type.count(list.component) == 0)
        {
            continue;
        }

        void* data_loc = list.get(record->row);
        ComponentList& new_list = new_archetype->components[ECS::component_index[list.component][new_archetype->id].column];
        void* new_data_loc = new_list.get(row);
        memcpy(new_data_loc,data_loc,list.element_size);
        list.remove(record->row);
    }
    record->archetype->entities.erase(record->row);
    record->archetype = new_archetype;
    record->row = row;
}

bool Entity::hasComponent(EntityID entity, ComponentID component) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    if(archetype == nullptr)
    {
        throw std::runtime_error("Somethings gone wrong. Archetype isn't supposed to be null!");
    }
    ArchetypeMap& archetype_map = ECS::component_index[component];
    return archetype_map.count(archetype->id) != 0;
}

bool Entity::hasComponent(EntityID entity, const std::string &component) {
    return hasComponent(entity,Component::getComponentID(component));
}

void* Entity::getComponent(EntityID entity, ComponentID component) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    if(archetype == nullptr)
    {
        throw std::runtime_error("Somethings gone wrong. Archetype isn't supposed to be null!");
    }
    ArchetypeMap& archetypes = ECS::component_index[component];
    if(archetypes.count(archetype->id) == 0)
    {
        return nullptr;
    }

    ArchetypeRecord& a_record = archetypes[archetype->id];
    return archetype->components[a_record.column].get(record->row);
}

void Entity::removeComponent(EntityID entity, ComponentID component) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    if(archetype == nullptr)
    {
        throw std::runtime_error("Somethings gone wrong. Archetype isn't supposed to be null!");
    }
    Archetype* next = archetype->removeComponent(component);
    moveEntityArchetype(entity, next);
}

void Entity::removeComponent(EntityID entity, const std::string &component) {
    removeComponent(entity,Component::getComponentID(component));
}

void* Entity::getComponent(EntityID entity, const std::string &component) {
    return getComponent(entity,Component::getComponentID(component));
}

void* Entity::addComponent(EntityID entity, ComponentID component) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    if(archetype == nullptr)
    {
        throw std::runtime_error("Somethings gone wrong. Archetype isn't supposed to be null!");
    }
    Archetype* next = archetype->addComponent(component);
    moveEntityArchetype(entity, next);
    return getComponent(entity,component);
}

void* Entity::addComponent(EntityID entity, const std::string &component) {
    return addComponent(entity,Component::getComponentID(component));
}

Entity::Entity() {
    this->id = 0;
}

Entity::Entity(EntityID id) {
    this->id = id;
}

Entity::Entity(const Entity &entity) {
    this->id = entity.id;
}

Entity& Entity::operator=(const Entity &entity) {
    this->id = entity.id;
    return *this;
}

bool Entity::operator==(const Entity &entity) const {
    return id == entity.id;
}

Entity::operator EntityID() const {
    return id;
}

bool Entity::exists() const {
    return id != 0;
}

bool Entity::hasComponent(ComponentID component) const {
    return hasComponent(id,component);
}

bool Entity::hasComponent(const std::string &component) const {
    return hasComponent(id,Component::getComponentID(component));
}

void *Entity::getComponent(ComponentID component) const {
    return getComponent(id,component);
}

void *Entity::getComponent(const std::string &component) const {
    return getComponent(id,Component::getComponentID(component));
}

void* Entity::addComponent(ComponentID component) const {
    return addComponent(id,component);
}

void* Entity::addComponent(const std::string &component) const {
    return addComponent(id,Component::getComponentID(component));
}

void Entity::removeComponent(ComponentID component) const {
    removeComponent(id,component);
}

void Entity::removeComponent(const std::string &component) const {
    removeComponent(id,Component::getComponentID(component));
}

Type Entity::getType() const {
    return ECS::entity_index[id]->archetype->type;
}
