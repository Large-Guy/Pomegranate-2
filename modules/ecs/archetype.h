#ifndef POMEGRANATEENGINE_ARCHETYPE_H
#define POMEGRANATEENGINE_ARCHETYPE_H
#include <component.h>
#include <unordered_set>
#include <unordered_map>
#include <ecs_typedefs.h>

struct VectorHash {
    std::size_t operator()(const std::vector<component_id>& v) const {
        std::size_t seed = 0;
        for (const component_id& i : v) {
            seed ^= i + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

struct Archetype {
    archetype_id _id;
    entity_type _type;
    std::unordered_set<component_id> _typeSet;
    static std::unordered_map<entity_type, Archetype*, VectorHash> _archetypeIndex;
    static std::unordered_map<component_id,std::unordered_set<archetype_id>> _componentIndex;

    explicit Archetype(entity_type type);
};

#endif //POMEGRANATEENGINE_ARCHETYPE_H
