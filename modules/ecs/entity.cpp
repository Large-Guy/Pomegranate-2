#include "entity.h"

#include <utility>

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
        list.add();
        row = list.count - 1;
    }
    for(auto& list : record->archetype->components)
    {
        void* data_loc = list.get(record->row);
        ComponentList& new_list = new_archetype->components[ECS::component_index[list.component][new_archetype->id].column];
        void* new_data_loc = new_list.get(row);
        memcpy(new_data_loc,data_loc,list.element_size);
        //TODO: Remove old data
    }
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

void Entity::addComponent(EntityID entity, ComponentID component) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    if(archetype == nullptr)
    {
        throw std::runtime_error("Somethings gone wrong. Archetype isn't supposed to be null!");
    }
    Archetype* next = archetype->addComponent(component);
    moveEntityArchetype(entity, next);
}

Entity::Entity() {
    this->id = ECS::entity_index.size();
    ECS::entity_index[this->id] = new EntityRecord{Archetype::getArchetype({}),0};
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

Entity::operator EntityID() const {
    return id;
}

bool Entity::hasComponent(ComponentID component) const {
    return hasComponent(id,component);
}

void *Entity::getComponent(ComponentID component) const {
    return getComponent(id,component);
}

void Entity::addComponent(ComponentID component) {
    addComponent(id,component);
}

