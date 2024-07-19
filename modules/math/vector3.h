#ifndef POMEGRANATEENGINE_VECTOR3_H
#define POMEGRANATEENGINE_VECTOR3_H
#include <serializable.h>

struct Vector3 : public Serializable {
    float x, y, z;
    Vector3();
    Vector3(float v);
    Vector3(float x, float y, float z = 0.0f);

    Vector3 operator+(const Vector3& v);
    Vector3 operator-(const Vector3& v);
    Vector3 operator*(const Vector3& v);
    Vector3 operator/(const Vector3& v);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_VECTOR3_H
