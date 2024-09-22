#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_NAME_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_NAME_H
#include <ecs/ecs.h>
#include <core/core.h>

struct Name : public Reflectable {
    std::string name;
    Name();
    Name(const std::string& name);
    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_NAME_H
