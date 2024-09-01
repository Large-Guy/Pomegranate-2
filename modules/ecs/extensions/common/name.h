#ifndef POMEGRANATEENGINE_NAME_H
#define POMEGRANATEENGINE_NAME_H
#include <ecs/ecs.h>
#include <core/core.h>

struct Name : public Component {
    String name;
    Name();
    Name(const String& name);
};


#endif //POMEGRANATEENGINE_NAME_H
