#ifndef POMEGRANATEENGINE_COLOR_H
#define POMEGRANATEENGINE_COLOR_H
#include <ecs/ecs.h>
#include <math/math.h>

struct Color : public Component {
    Vector4 color;
    Color();
    Color(const Vector4& color);
};


#endif //POMEGRANATEENGINE_COLOR_H
