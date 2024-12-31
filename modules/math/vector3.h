#ifndef POMEGRANATE_ENGINE_MATH_VECTOR3_H
#define POMEGRANATE_ENGINE_MATH_VECTOR3_H
#include <core/serializable.h>
#include <cmath>
#include <array>

struct Vector3{
    static const Vector3 zero;
    static const Vector3 one;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 left;
    static const Vector3 right;
    static const Vector3 forward;
    static const Vector3 back;

    float x = 0, y = 0, z = 0;
    Vector3();
    Vector3(float v);
    Vector3(float x, float y, float z = 0.0f);

    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(const Vector3& v) const;
    Vector3 operator/(const Vector3& v) const;
    Vector3 operator*(float v) const;
    Vector3 operator/(float v) const;
    void operator+=(const Vector3& v);
    void operator-=(const Vector3& v);
    void operator*=(const Vector3& v);
    void operator/=(const Vector3& v);
    void operator*=(float v);
    void operator/=(float v);
    bool operator==(const Vector3& v) const;

    [[nodiscard]] float dot(const Vector3& v) const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector3 normalize() const;
    [[nodiscard]] Vector3 lerp(const Vector3& v, float t) const;
    [[nodiscard]] Vector3 slerp(const Vector3& v, float t) const;
    [[nodiscard]] Vector3 nlerp(const Vector3& v, float t) const;
    [[nodiscard]] Vector3 cross(const Vector3& v) const;
    [[nodiscard]] Vector3 reflect(const Vector3& normal) const;
    [[nodiscard]] Vector3 refract(const Vector3& normal, float eta) const;
    [[nodiscard]] Vector3 rotate(float angle) const;
    [[nodiscard]] Vector3 rotate(const Vector3& rotation) const;
    //Vector3 rotate(float angle) const;
    //Vector3 rotate(const Vector3& pivot, float angle) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);

    [[nodiscard]] std::array<float, 3> get() const;
};

template<>
struct std::hash<Vector3> {
    size_t operator()(const Vector3& v) const
    {
        return hash<float>()(v.x) ^ hash<float>()(v.y) ^ hash<float>()(v.z);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_VECTOR3_H
