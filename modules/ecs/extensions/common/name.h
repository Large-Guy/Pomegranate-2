#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_NAME_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_NAME_H
#include <ecs/ecs.h>
#include <core/core.h>

struct Name : public Component {
    String name;
    Name();
    Name(const String& name);
};


#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_NAME_H
