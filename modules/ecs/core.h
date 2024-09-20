#ifndef POMEGRANATE_ENGINE_ECS_CORE_H
#define POMEGRANATE_ENGINE_ECS_CORE_H
#include <unordered_map>
#include <string>
#include <core/core.h>
#include "ecs_typedefs.h"
#include <functional>
#ifndef __APPLE__
#include <omp.h>
#endif

struct Entity;
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
    static std::unordered_map<size_t, ComponentID> component_ids;
    static int threadCount;

    template <typename Args>
    static void parallelEach(ComponentID component, std::function<void(Args*)> func);

    template <typename Args>
    static void parallelEach(const std::string& component, std::function<void(Args*)> func);

    template <typename Args>
    static void parallelEach(ComponentID component, std::function<void(Args*, Entity&)> func);

    template <typename Args>
    static void parallelEach(const std::string& component, std::function<void(Args*, Entity&)> func);

    static void parallelEach(ComponentID component, std::function<void(void*)> func);

    static void parallelEach(ComponentID component, std::function<void(void*, Entity&)> func);

    static void parallelEach(const std::string& component, std::function<void(void*)> func);

    static void parallelEach(const std::string& component, std::function<void(void*, Entity&)> func);

    template <typename Args>
    static void each(ComponentID component, std::function<void(Args*)> func);

    template <typename Args>
    static void each(const std::string& component, std::function<void(Args*)> func);

    template <typename Args>
    static void each(ComponentID component, std::function<void(Args*, Entity&)> func);

    template <typename Args>
    static void each(const std::string& component, std::function<void(Args*, Entity&)> func);

    static void each(ComponentID component, std::function<void(void*)> func);

    static void each(const std::string& component, std::function<void(void*)> func);

    static void each(ComponentID component, std::function<void(void*, Entity&)> func);

    static void each(const std::string& component, std::function<void(void*, Entity&)> func);

    static void setThreadCount(int count);
    static unsigned int getMaxThreadCount();
};

#endif //POMEGRANATE_ENGINE_CORE_H
