#ifndef POMEGRANATEENGINE_ARCHETYPE_H
#define POMEGRANATEENGINE_ARCHETYPE_H
#include <unordered_set>
#include <unordered_map>
#include "ecs_typedefs.h"
#include <cstring>
#include "component.h"
#include "core.h"

struct Archetype;

struct ArchetypeRecord {
    size_t column;
    Archetype* archetype;
};

struct ArchetypeEdge{
    Archetype* add;
    Archetype* remove;
};

struct Archetype {
    ArchetypeID id;
    Type type;
    std::vector<ComponentList> components;
    std::unordered_map<ComponentID, ArchetypeEdge> edges;
    static Archetype* getArchetype(const Type& type);

    Archetype* addComponent(ComponentID component);
    Archetype* removeComponent(ComponentID component);
    void removeRow(size_t row);
};

#endif //POMEGRANATEENGINE_ARCHETYPE_H
