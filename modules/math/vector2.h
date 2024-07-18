#ifndef POMEGRANATEENGINE_VEC2_H
#define POMEGRANATEENGINE_VEC2_H
#include "serializable.h"

struct Vector2 : public Serializable {
    float x, y;
    Vector2();
    Vector2(float v);
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& v);
    Vector2 operator-(const Vector2& v);
    Vector2 operator*(const Vector2& v);
    Vector2 operator/(const Vector2& v);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) const override;
};


#endif //POMEGRANATEENGINE_VEC2_H
