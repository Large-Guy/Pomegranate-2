#include "core.h"

#include "entity.h"

std::unordered_map<EntityID, EntityRecord*> ECS::entity_index;
std::unordered_map<Type, Archetype*, SetHash, SetEqual> ECS::archetype_index;
std::unordered_map<ComponentID,ArchetypeMap> ECS::component_index;
std::unordered_map<ComponentID, size_t> ECS::component_sizes;
std::unordered_map<std::string, ComponentID> ECS::component_names;

std::size_t SetHash::operator()(const std::unordered_set<EntityID>& set) const {
    std::size_t hash_value = 0;
    std::hash<EntityID> hasher;

    for (EntityID id : set) {
        hash_value ^= hasher(id) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    }

    return hash_value;
}

bool SetEqual::operator()(const std::unordered_set<EntityID>& set1, const std::unordered_set<EntityID>& set2) const {
    if (set1.size() != set2.size()) {
        return false;
    }

    for (const EntityID& id : set1) {
        if (set2.find(id) == set2.end()) {
            return false;
        }
    }

    return true;
}

ComponentID ECS::registerComponent(const std::string& component, size_t size) {
    ComponentID id = component_sizes.size();
    component_sizes[id] = size;
    component_names[component] = id;
    return id;
}

ComponentID ECS::getComponentID(const std::string &component) {
    if(component_names.find(component) == component_names.end())
    {
        throw std::runtime_error("Component \"" + component + "\" not found!");
    }
    return component_names[component];
}

std::string ECS::getComponentName(ComponentID component) {
    for(auto& pair : component_names)
    {
        if(pair.second == component)
        {
            return pair.first;
        }
    }
    throw std::runtime_error("Component with ID " + std::to_string(component) + " not found!");
}