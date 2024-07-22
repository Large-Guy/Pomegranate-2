#ifndef POMEGRANATEENGINE_ARCHETYPE_H
#define POMEGRANATEENGINE_ARCHETYPE_H
#include <component.h>
#include <unordered_set>
#include <unordered_map>
#include <ecs_typedefs.h>

class Archetype {
private:

    struct VectorHash {
        std::size_t operator()(const std::vector<ComponentID>& vec) const;
    };

    uint _id;
    static uint _archetypeCount;
public:
    std::unordered_set<ComponentID> _type;
    std::vector<ComponentID> _components;
    static std::unordered_map<std::vector<ComponentID>,Archetype*,VectorHash> archetypes;

    Archetype();

    // Getters
    [[nodiscard]] uint getId() const;

    Archetype* addComponent(ComponentID new_component);
};


#endif //POMEGRANATEENGINE_ARCHETYPE_H
