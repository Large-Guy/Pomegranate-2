#ifndef POMEGRANATEENGINE_ECS_H
#define POMEGRANATEENGINE_ECS_H
#include <unordered_map>

#include "ecs_typedefs.h"

struct EntityRecord;
struct ArchetypeRecord;
struct Archetype;

struct SetHash {
    std::size_t operator()(const std::unordered_set<EntityID>& set) const;
};

struct SetEqual {
    bool operator()(const std::unordered_set<EntityID>& set1, const std::unordered_set<EntityID>& set2) const;
};

class ECS {
public:
    static std::unordered_map<EntityID, EntityRecord*> entity_index;
    static std::unordered_map<Type, Archetype*, SetHash, SetEqual> archetype_index;
    static std::unordered_map<ComponentID,ArchetypeMap> component_index;
    static std::unordered_map<ComponentID, size_t> component_sizes;

    static ComponentID registerComponent(size_t size);
};

#endif //POMEGRANATEENGINE_ECS_H
