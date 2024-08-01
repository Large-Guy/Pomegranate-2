#ifndef POMEGRANATEENGINE_VECTOR4_H
#define POMEGRANATEENGINE_VECTOR4_H
#include <serializable.h>

struct Vector4 : public Serializable {
    float x = 0, y = 0, z = 0, w = 0;
    Vector4();
    Vector4(float v);
    Vector4(float x, float y, float z, float w = 0.0f);

    Vector4 operator+(const Vector4& v);
    Vector4 operator-(const Vector4& v);
    Vector4 operator*(const Vector4& v);
    Vector4 operator/(const Vector4& v);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;

};


#endif //POMEGRANATEENGINE_VECTOR4_H
