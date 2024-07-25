#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<serializable.h>
#include<archetype.h>
#include <ecs_typedefs.h>

struct EntityRecord
{
    Archetype* archetype;
    size_t row;
};

class Entity
{
private:
    void moveEntityArchetype(Archetype* current, size_t row, Archetype* next, component_id component);
public:
    entity_id _id;
    static std::unordered_map<entity_id, EntityRecord> _entityIndex;

    Entity();
    ~Entity();

    bool hasComponent(component_id component) const;
    template <typename T> void addComponent(component_id component, T data);
    template <typename T> T* getComponent(component_id component);
};

template<typename T>
void Entity::addComponent(component_id component, T data) {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id]; //Get current record
    Archetype* new_archetype = eRecord.archetype->addComponent(component); //Find the new archetype of the entity with new component
    moveEntityArchetype(eRecord.archetype,eRecord.row,new_archetype,component); //Move the archetype
    eRecord = {new_archetype,new_archetype->_components[0].data.size() - 1}; //Set the record to the new archetype and the new row
    new_archetype->_components[Archetype::_componentIndex[component][new_archetype->_id].column].data[eRecord.row] = new T(data); //Set the new component data
}

template <typename T> T*
Entity::getComponent(component_id component) {
    EntityRecord& eRecord = Entity::_entityIndex[this->_id];
    Archetype*& archetype = eRecord.archetype;
    std::unordered_map<archetype_id,ComponentLocation> archetypes = Archetype::_componentIndex[component];
    if(archetypes.count(archetype->_id) == 0) {
        return nullptr;
    }

    ComponentLocation& aRecord = archetypes[archetype->_id];
    return (T*)archetype->_components[aRecord.column].data[eRecord.row];
}
#endif //POMEGRANATEENGINE_ENTITY_H
