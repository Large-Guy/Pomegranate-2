#ifndef POMEGRANATE_ENGINE_ECS_CORE_IMPL_H
#define POMEGRANATE_ENGINE_ECS_CORE_IMPL_H

#include <ecs/ecs.h>
#include <omp.h>

template <typename Args>
void ECS::each(ComponentID component, std::function<void(Args*)> func)
{
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
#pragma omp parallel for
        for(size_t i = 0; i < record.archetype->components[record.column].count; i++)
        {
            //Call the function
            func((Args*)record.archetype->components[record.column].get(i));
        }
    }
}

template <typename Args>
void ECS::each(const std::string& component, std::function<void(Args*)> func)
{
    each(Component::getComponentID(component), func);
}

template <typename Args>
void ECS::each(ComponentID component, std::function<void(Args*,Entity&)> func)
{
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
#pragma omp parallel for
        for(size_t i = 0; i < record.archetype->components[record.column].count; i++)
        {
            //Call the function
            Entity entity(record.archetype->entities[i]);
            func((Args*)record.archetype->components[record.column].get(i),entity);
        }
    }
}

template <typename Args>
void ECS::each(const std::string& component, std::function<void(Args*,Entity&)> func)
{
    each(Component::getComponentID(component), func);
}

#endif //POMEGRANATE_ENGINE_ECS_IMPL_H
