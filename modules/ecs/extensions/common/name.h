#ifndef POMEGRANATEENGINE_NAME_H
#define POMEGRANATEENGINE_NAME_H
#include <ecs/ecs.h>

struct Name : public Component {
    std::string name;
    Name();
    Name(const std::string& name);
};


#endif //POMEGRANATEENGINE_NAME_H
