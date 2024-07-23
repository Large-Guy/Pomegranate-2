#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<serializable.h>
#include<component.h>
#include<archetype.h>
#include <ecs_typedefs.h>

class Entity
{
public:
    entity_id _id;
    static std::unordered_map<entity_id, Archetype*> _entityIndex;

    Entity();

    bool hasComponent(component_id component);
};

#endif //POMEGRANATEENGINE_ENTITY_H
