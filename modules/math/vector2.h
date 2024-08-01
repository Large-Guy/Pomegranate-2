#ifndef POMEGRANATEENGINE_VEC2_H
#define POMEGRANATEENGINE_VEC2_H
#include "serializable.h"

struct Vector2 : public Serializable {
    float x = 0, y = 0;
    Vector2();
    Vector2(float v);
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& v);
    Vector2 operator-(const Vector2& v);
    Vector2 operator*(const Vector2& v);
    Vector2 operator/(const Vector2& v);
    explicit operator std::string() const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_VEC2_H
