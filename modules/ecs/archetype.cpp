#include "archetype.h"

Archetype* Archetype::getArchetype(const Type& type) {
    if(ECS::archetype_index.find(type) == ECS::archetype_index.end())
    {
        Archetype* archetype = new Archetype();
        archetype->type = type;
        archetype->id = ECS::archetype_index.size();
        ECS::archetype_index[type] = archetype;
        for(auto c : type)
        {
            archetype->components.emplace_back(c,ECS::component_sizes[c]);
            ECS::component_index[c][archetype->id] = {archetype->components.size()-1};
        }
    }
    return ECS::archetype_index[type];
}

Archetype* Archetype::addComponent(ComponentID component) {
    if(edges.find(component) != edges.end())
    {
        ArchetypeEdge& edge = edges[component];
        if(edge.add != nullptr)
        {
            return edge.add;
        }
        else
        {
            Type new_type = type;
            type.insert(component);
            edge.add = getArchetype(type);
            return edge.add;
        }
    }
    else
    {
        Type new_type = type;
        new_type.insert(component);
        ArchetypeEdge& edge = edges[component];
        edge.add = getArchetype(new_type);
        return edge.add;
    }
}

void Archetype::removeRow(size_t row) {
    for(auto& component : components)
    {
        component.remove(row);
    }
}