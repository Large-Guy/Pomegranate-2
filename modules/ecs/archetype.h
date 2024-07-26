#ifndef POMEGRANATEENGINE_ARCHETYPE_H
#define POMEGRANATEENGINE_ARCHETYPE_H
#include <unordered_set>
#include <unordered_map>
#include <ecs_typedefs.h>

struct Group;

struct Archetype;

struct VectorHash {
    std::size_t operator()(const std::vector<component_id>& v) const {
        //Symmetric polynomial hash
        //Sort the vector
        std::vector<component_id> v_copy = v;
        std::sort(v_copy.begin(), v_copy.end());
        std::size_t seed = v.size();
        for(auto& i : v_copy) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct VectorComparison {
    bool operator()(const std::vector<component_id>& lhs, const std::vector<component_id>& rhs) const {
        //Sort the vectors

        std::vector<component_id> lhs_copy = lhs;
        std::sort(lhs_copy.begin(), lhs_copy.end());
        std::vector<component_id> rhs_copy = rhs;
        std::sort(rhs_copy.begin(), rhs_copy.end());
        return lhs_copy == rhs_copy;
    }
};
struct ArchetypeNode{
    Archetype* add;
    Archetype* remove;
};

struct ComponentLocation {
    size_t column;
};

struct Column {
    component_id component;
    size_t componentSize;
    std::vector<void*> data;
};

struct Archetype {
private:
    archetype_id _id;
    entity_type _type;
    std::unordered_set<component_id> _typeSet;
    std::unordered_map<component_id, ArchetypeNode> _nodes;
    std::vector<Column> _components;

    static std::unordered_map<entity_type, Archetype*, VectorHash, VectorComparison> _archetypeIndex;
    static std::unordered_map<component_id,std::unordered_map<archetype_id, ComponentLocation>> _componentIndex;

public:
    explicit Archetype(entity_type type);

    Archetype* addComponent(component_id component);

    friend class Entity;
    friend class Component;
    friend class Group;
};

#endif //POMEGRANATEENGINE_ARCHETYPE_H
