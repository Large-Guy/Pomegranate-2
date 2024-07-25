#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<serializable.h>
#include<component.h>
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
    void addComponent(component_id component);
    void* getComponent(component_id component) const;
};

#endif //POMEGRANATEENGINE_ENTITY_H
