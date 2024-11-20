#ifndef POMEGRANATE_ENGINE_MATH_VECTOR4_H
#define POMEGRANATE_ENGINE_MATH_VECTOR4_H
#include <core/serializable.h>
#include <cmath>
#include <array>

struct Vector4 {
    float x = 0, y = 0, z = 0, w = 0;
    Vector4();
    Vector4(float v);
    Vector4(float x, float y, float z, float w = 0.0f);

    Vector4 operator+(const Vector4& v) const;
    Vector4 operator-(const Vector4& v) const;
    Vector4 operator*(const Vector4& v) const;
    Vector4 operator/(const Vector4& v) const;
    Vector4 operator*(float v) const;
    Vector4 operator/(float v) const;
    void operator+=(const Vector4& v);
    void operator-=(const Vector4& v);
    void operator*=(const Vector4& v);
    void operator/=(const Vector4& v);
    void operator*=(float v);
    void operator/=(float v);
    bool operator==(const Vector4& v) const;

    [[nodiscard]] float dot(const Vector4& v) const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector4 normalize() const;
    [[nodiscard]] Vector4 lerp(const Vector4& v, float t) const;
    [[nodiscard]] Vector4 slerp(const Vector4& v, float t) const;
    [[nodiscard]] Vector4 nlerp(const Vector4& v, float t) const;
    [[nodiscard]] Vector4 cross(const Vector4& v) const;
    [[nodiscard]] Vector4 reflect(const Vector4& normal) const;
    [[nodiscard]] Vector4 refract(const Vector4& normal, float eta) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);

    [[nodiscard]] std::array<float, 4> get() const;
};

template<>
struct std::hash<Vector4> {
    size_t operator()(const Vector4& v) const
    {
        return hash<float>()(v.x) ^ hash<float>()(v.y) ^ hash<float>()(v.z) ^ hash<float>()(v.w);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_VECTOR4_H
