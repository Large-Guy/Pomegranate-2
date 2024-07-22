#include "archetype.h"

uint Archetype::_archetypeCount = 0;
std::unordered_map<std::vector<ComponentID>,Archetype*,Archetype::VectorHash> Archetype::archetypes;

std::size_t Archetype::VectorHash::operator()(const std::vector<ComponentID> &vec) const {
    std::size_t seed = 0;
    for (const auto& component : vec) {
        // Combine hash values of each component into seed
        seed ^= std::hash<ComponentID>()(component);
    }
    return seed;
}

Archetype::Archetype() {
    _id = _archetypeCount++;
    _components = std::vector<ComponentID>();
}

uint Archetype::getId() const {
    return _id;
}

Archetype *Archetype::addComponent(ComponentID new_component) {
    std::vector<ComponentID> components = {};
    for(auto c : _components)
    {
        components.push_back(c);
    }
    components.push_back(new_component);
    if(archetypes.find(components) == archetypes.end())
    {
        auto* n = new Archetype();
        n->_type.insert(components.begin(), components.end());
        n->_components.insert(n->_components.end(),components.begin(),components.end());
        for(auto c : components)
        {
            Component::components[c].insert(n->getId());
        }
        archetypes[components] = n;
        return n;
    }
    else
    {
        return archetypes[components];
    }
}

