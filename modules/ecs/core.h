#ifndef POMEGRANATEENGINE_CORE_H
#define POMEGRANATEENGINE_CORE_H
#include <unordered_map>
#include <string>
#include "ecs_typedefs.h"
#include <functional>

struct EntityRecord;
struct ArchetypeRecord;
struct Archetype;

struct SetHash {
    std::size_t operator()(const std::unordered_set<EntityID>& set) const;
};

struct SetEqual {
    bool operator()(const std::unordered_set<EntityID>& set1, const std::unordered_set<EntityID>& set2) const;
};

class ECS {
public:
    static std::unordered_map<EntityID, EntityRecord*> entity_index;
    static std::unordered_map<Type, Archetype*, SetHash, SetEqual> archetype_index;
    static std::unordered_map<ComponentID,ArchetypeMap> component_index;
    static std::unordered_map<ComponentID, size_t> component_sizes;
    static std::unordered_map<std::string, ComponentID> component_names;

    static ComponentID registerComponent(const std::string& component,size_t size);
    template<typename T>
    static ComponentID registerComponent(const std::string& component);
    static ComponentID getComponentID(const std::string& component);
    static std::string getComponentName(ComponentID component);

    template <typename Args>
    static void each(ComponentID component, std::function<void(Args*)> func);

    template <typename Args>
    static void each(const std::string& component, std::function<void(Args*)> func);
};

#endif //POMEGRANATEENGINE_CORE_H
