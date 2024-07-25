#include "archetype.h"

#include <utility>

std::unordered_map<entity_type, Archetype*, VectorHash, VectorComparison> Archetype::_archetypeIndex;
std::unordered_map<component_id,std::unordered_map<archetype_id, ComponentLocation>> Archetype::_componentIndex;

Archetype::Archetype(entity_type type) {
    _id = Archetype::_archetypeIndex.size();
    _type = std::move(type);
    Archetype::_archetypeIndex[_type] = this;
    for(auto& c : _type)
    {
        Archetype::_componentIndex[c][_id].column = _components.size();
        _components.push_back({c,0,{}});
    }
}

Archetype* Archetype::addComponent(component_id component) {
    ArchetypeNode& node = _nodes[component];
    if(node.add == nullptr)
    {
        entity_type type = _type;
        type.push_back(component);
        Archetype*& archetype = Archetype::_archetypeIndex[type];
        if(archetype == nullptr)
        {
            archetype = new Archetype(type);
        }
        node.add = archetype;
    }
    return node.add;
}
