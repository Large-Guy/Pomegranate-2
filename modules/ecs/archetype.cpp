#include "archetype.h"

#include <utility>

std::unordered_map<entity_type, Archetype*, VectorHash, VectorComparison> Archetype::_archetypeIndex;
std::unordered_map<component_id,std::unordered_map<archetype_id, ComponentLocation>> Archetype::_componentIndex;

ArchetypeNode::ArchetypeNode() {
    add = nullptr;
    remove = nullptr;
}

Archetype::Archetype(entity_type type) {
    _id = Archetype::_archetypeIndex.size();
    _type = std::move(type);
    Archetype::_archetypeIndex[_type] = this;
    for(auto& c : _type)
    {
        Archetype::_componentIndex[c][_id].column = _components.size();
        _components.emplace_back(c);
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
        for (auto& t : _type) {
            size_t my_component_column = Archetype::_componentIndex[t][_id].column;
            size_t size = _components[my_component_column].componentSize;
            size_t component_column = Archetype::_componentIndex[t][archetype->_id].column;
            archetype->_components[component_column].setComponentSize(size);
        }
        node.add = archetype;
    }
    return node.add;
}