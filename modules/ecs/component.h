#ifndef POMEGRANATEENGINE_COMPONENT_H
#define POMEGRANATEENGINE_COMPONENT_H
#include <serializable.h>
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <any>
#include <functional>
#include <ecs_typedefs.h>

class Component : public Serializable {
private:
    static uint _componentCount;
public:
    static std::unordered_map<ComponentID,std::unordered_set<ArchetypeID>> components;
    static std::unordered_map<std::string,ComponentID> componentTypes;
};



#endif //POMEGRANATEENGINE_COMPONENT_H
