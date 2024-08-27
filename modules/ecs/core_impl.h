#ifndef POMEGRANATEENGINE_ECS_IMPL_H
#define POMEGRANATEENGINE_ECS_IMPL_H

#include <ecs/ecs.h>

template <typename T>
ComponentID ECS::registerComponent(const std::string& component)
{
    return ECS::registerComponent(component, sizeof(T));
}

template <typename Args>
void ECS::each(ComponentID component, std::function<void(Args*)> func)
{
    for(auto archetype : ECS::component_index[component])
    {
        ArchetypeRecord& record = archetype.second;
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
    each(getComponentID(component), func);
}

#endif //POMEGRANATEENGINE_ECS_IMPL_H
