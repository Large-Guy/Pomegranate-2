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
    Debug::AssertIf::isNull(old_archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");
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
        if(ECS::functions.find(list.component) != ECS::functions.end())
        {
            if(ECS::functions[list.component].copy != nullptr)
            {
                ECS::functions[list.component].copy(new_data_loc,data_loc);
            }
            else
            {
                memcpy(new_data_loc,data_loc,list.element_size);
            }
        }
        list.remove(record->row);
    }
    record->archetype->entities.erase(record->row);
    record->archetype = new_archetype;
    record->row = row;
}

bool Entity::hasComponent(EntityID entity, ComponentID component) {
    if(ECS::component_index.find(component) == ECS::component_index.end())
    {
        return false;
    }
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    Debug::AssertIf::isNull(archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");
    ArchetypeMap& archetype_map = ECS::component_index[component];
    return archetype_map.count(archetype->id) != 0;
}

bool Entity::hasComponent(EntityID entity, const std::string &component) {
    if(ECS::component_names.find(component) == ECS::component_names.end())
    {
        return false;
    }
    return hasComponent(entity,Component::getComponentID(component));
}

void* Entity::getComponent(EntityID entity, ComponentID component) {
    EntityRecord* record = ECS::entity_index[entity];
    Archetype* archetype = record->archetype;
    Debug::AssertIf::isNull(archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");
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
    Debug::AssertIf::isNull(archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");
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
    Debug::AssertIf::isNull(archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");
    Archetype* next = archetype->addComponent(component);
    moveEntityArchetype(entity, next);
    void* data = getComponent(entity,component);
    if(ECS::functions.find(component) != ECS::functions.end())
    {
        if(ECS::functions[component].constructor != nullptr)
            ECS::functions[component].constructor(data);
    }
    return data;
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

Entity::Entity(const Entity *entity) {
    this->id = entity->id;
}

Entity& Entity::operator=(const Entity &entity) {
    this->id = entity.id;
    return *this;
}

Entity& Entity::operator=(const EntityID &entity) {
    this->id = entity;
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

bool Entity::has(ComponentID component) const {
    return hasComponent(id,component);
}

bool Entity::has(const std::string &component) const {
    return hasComponent(id,Component::getComponentID(component));
}

void *Entity::get(ComponentID component) const {
    void* data = getComponent(id,component);

    return data;
}

void *Entity::get(const std::string &component) const {
    return getComponent(id,Component::getComponentID(component));
}

void* Entity::add(ComponentID component) const {
    return addComponent(id,component);
}

void* Entity::add(const std::string &component) const {
    return addComponent(id,Component::getComponentID(component));
}

void Entity::remove(ComponentID component) const {
    removeComponent(id,component);
}

void Entity::remove(const std::string &component) const {
    removeComponent(id,Component::getComponentID(component));
}

Type Entity::getType() const {
    return ECS::entity_index[id]->archetype->type;
}

void Entity::destroy() {
    //We have to delete all the components
    EntityRecord* record = ECS::entity_index[id];
    Archetype* archetype = record->archetype;
    Debug::AssertIf::isNull(archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");
    for(auto& list : archetype->components)
    {
        void* data = list.get(record->row);
        if(ECS::functions.find(list.component) != ECS::functions.end())
        {
            if(ECS::functions[list.component].destructor != nullptr)
                ECS::functions[list.component].destructor(data);
        }
    }
    archetype->removeRow(record->row);
    ECS::entity_index.erase(id);
    delete record;
    id = NULL_ENTITY;
}

void Entity::serialize(Archive &archive) const {
    if(id == NULL_ENTITY)
    {
        return;
    }

    EntityRecord* record = ECS::entity_index[id];
    Archetype* archetype = record->archetype;
    Debug::AssertIf::isNull(archetype, "Somethings gone wrong. Most likely an engine bug. Sorry!");

    for(auto& list : archetype->components)
    {
        void* componentData = list.get(record->row);
        archive << list.component;
        Archive component{};
        ECS::functions[list.component].serialize(component,componentData);
        archive << component;
    }
}

void Entity::deserialize(Archive &archive) {
    while(!archive.isEnd())
    {
        ComponentID component;
        archive >> component;
        Archive component_archive;
        archive >> component_archive;
        void* data = addComponent(id,component);
        void* reflectable = (Reflectable*)data;
        ECS::functions[component].deserialize(component_archive,reflectable);
    }
}
