#include "component.h"

uint Component::_componentCount = 0;
std::unordered_map<uint,std::unordered_set<ComponentID>> Component::components;
std::unordered_map<std::string,uint> Component::componentTypes;