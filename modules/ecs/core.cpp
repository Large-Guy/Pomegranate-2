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

void ECS::setThreadCount(int count) {
#ifndef __APPLE__
    if(count == omp_get_max_threads())
    {
        Debug::Log::warn("Using maximum number of threads may lead to diminishing returns. Consider using a smaller number of threads.");
    }
    omp_set_num_threads(count);
#endif
}

int ECS::getMaxThreadCount() {
#ifndef __APPLE__
    return omp_get_max_threads();
#else
    return 1;
#endif
}

void ECS::parallelEach(ComponentID component, std::function<void(void *)> func) {
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
#pragma omp parallel for
        for(size_t i = 0; i < record.archetype->components[record.column].count; i++)
        {
            //Call the function
            func(record.archetype->components[record.column].get(i));
        }
    }
#pragma omp barrier
}

void ECS::parallelEach(const std::string &component, std::function<void(void *)> func) {
    parallelEach(Component::getComponentID(component), func);
}

void ECS::parallelEach(ComponentID component, std::function<void(void *, Entity &)> func) {
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
#pragma omp parallel for
        for(size_t i = 0; i < record.archetype->components[record.column].count; i++)
        {
            //Call the function
            Entity entity(record.archetype->entities[i]);
            func(record.archetype->components[record.column].get(i),entity);
        }
    }
#pragma omp barrier
}

void ECS::parallelEach(const std::string &component, std::function<void(void *, Entity &)> func) {
    parallelEach(Component::getComponentID(component), func);
}

void ECS::each(ComponentID component, std::function<void(void *)> func) {
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
        for(size_t i = 0; i < record.archetype->components[record.column].count; i++)
        {
            //Call the function
            func(record.archetype->components[record.column].get(i));
        }
    }
}

void ECS::each(const std::string &component, std::function<void(void *)> func) {
    each(Component::getComponentID(component), func);
}

void ECS::each(ComponentID component, std::function<void(void *, Entity &)> func) {
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
        for(size_t i = 0; i < record.archetype->components[record.column].count; i++)
        {
            //Call the function
            Entity entity(record.archetype->entities[i]);
            func(record.archetype->components[record.column].get(i),entity);
        }
    }
}

void ECS::each(const std::string &component, std::function<void(void *, Entity &)> func) {
    each(Component::getComponentID(component), func);
}
