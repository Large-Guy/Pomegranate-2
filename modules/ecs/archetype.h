#ifndef POMEGRANATEENGINE_ARCHETYPE_H
#define POMEGRANATEENGINE_ARCHETYPE_H
#include <unordered_set>
#include <unordered_map>
#include <ecs_typedefs.h>
#include <cstring>
#include <component.h>

struct Group;

struct Archetype;

struct SetHash {
    std::size_t operator()(const entity_type& v) const {
        std::size_t seed = v.size();
        for(auto& i : v) {
            seed += std::hash<component_id>{}(i);
        }
        return seed;
    }
};

struct SetComparison {
    bool operator()(const entity_type& lhs, const entity_type& rhs) const {
        if(lhs.size() != rhs.size()) return false;
        for(auto& i : lhs) {
            if(rhs.find(i) == rhs.end()) return false;
        }
        return true;
    }
};
struct ArchetypeNode{
    Archetype* add = nullptr;
    Archetype* remove = nullptr;
    ArchetypeNode();
};

struct ComponentLocation {
    size_t column = 0;
};

struct Archetype {
private:
    archetype_id _id = 0;
    entity_type _type = {};
    std::unordered_set<component_id> _typeSet = {};
    std::unordered_map<component_id, ArchetypeNode> _nodes = {};
    std::vector<ComponentList> _components = {};

    static std::unordered_map<entity_type, Archetype*, SetHash, SetComparison> _archetypeIndex;
    static std::unordered_map<component_id,std::unordered_map<archetype_id, ComponentLocation>> _componentIndex;

public:
    explicit Archetype(entity_type type);

    Archetype* addComponent(component_id component);

    friend class Entity;
    friend class Group;
};

#endif //POMEGRANATEENGINE_ARCHETYPE_H
