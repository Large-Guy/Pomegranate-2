#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<serializable.h>
#include<archetype.h>
#include <ecs_typedefs.h>
#include "component.h"

struct Group;

struct EntityRecord
{
    Archetype* archetype;
    size_t row;

    EntityRecord() = default;
    EntityRecord(Archetype* archetype, size_t row) : archetype(archetype), row(row) {}

    //Copy and move
    EntityRecord(const EntityRecord& other) = default;
    EntityRecord(EntityRecord&& other) noexcept = default;
    EntityRecord& operator=(const EntityRecord& other) = default;
    EntityRecord& operator=(EntityRecord&& other) noexcept = default;
};

class Entity
{
private:
    void moveEntityArchetype(Archetype* current, size_t row, Archetype* next, component_id component);
public:
    entity_id _id = 0;
    static std::unordered_map<entity_id, EntityRecord> _entityIndex;
    static entity_id _entityCount;

    Entity();
    ~Entity();

    bool hasComponent(component_id component) const;
    template <typename T> T* addComponent(component_id component);
    template <typename T> T* getComponent(component_id component);

    friend class Group;
};

template<typename T>
T* Entity::addComponent(component_id component) {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id]; //Get current record
    Archetype* new_archetype = eRecord.archetype->addComponent(component); //Find the new archetype of the entity with new component
    moveEntityArchetype(eRecord.archetype,eRecord.row,new_archetype,component); //Move the archetype
    ComponentList& list = new_archetype->_components[Archetype::_componentIndex[component][new_archetype->_id].column];
    if(!list.hasSize()) {
        list.setComponentSize(sizeof(Component<T>));
    }
    new ((Component<T>*)list.add()) Component<T>();
    return &(((Component<T>*)list.get(list.componentCount-1))[0]._data);
}

template <typename T> T*
Entity::getComponent(component_id component) {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id];
    Archetype*& archetype = eRecord.archetype;
    std::unordered_map<archetype_id,ComponentLocation>& archetypes = Archetype::_componentIndex[component];
    if(archetypes.count(archetype->_id) == 0) {
        return nullptr;
    }

    ComponentLocation& aRecord = archetypes[archetype->_id];
    return &(((Component<T>*)archetype->_components[aRecord.column].data)[eRecord.row])._data;
}

#endif //POMEGRANATEENGINE_ENTITY_H
