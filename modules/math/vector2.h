#ifndef POMEGRANATE_ENGINE_MATH_VEC2_H
#define POMEGRANATE_ENGINE_MATH_VEC2_H
#include <core/serializable.h>
#include <cmath>

struct Vector2 {
    float x = 0, y = 0;
    Vector2();
    Vector2(float v);
    Vector2(float x, float y);
    Vector2(const Vector2& other);

    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(const Vector2& v) const;
    Vector2 operator/(const Vector2& v) const;
    Vector2 operator*(float v) const;
    Vector2 operator/(float v) const;
    void operator+=(const Vector2& v);
    void operator-=(const Vector2& v);
    void operator*=(const Vector2& v);
    void operator/=(const Vector2& v);
    void operator*=(float v);
    void operator/=(float v);
    Vector2& operator=(const Vector2& v);
    bool operator==(const Vector2& v) const;

    [[nodiscard]] float dot(const Vector2& v) const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector2 normalized() const;
    [[nodiscard]] Vector2 lerp(const Vector2& v, float t) const;
    [[nodiscard]] Vector2 slerp(const Vector2& v, float t) const;
    [[nodiscard]] Vector2 nlerp(const Vector2& v, float t) const;
    [[nodiscard]] Vector2 cross(const Vector2& v) const;
    [[nodiscard]] Vector2 reflect(const Vector2& normal) const;
    [[nodiscard]] Vector2 refract(const Vector2& normal, float eta) const;
    [[nodiscard]] Vector2 rotate(float angle) const;
    [[nodiscard]] Vector2 rotate(const Vector2& pivot, float angle) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};

template<>
struct std::hash<Vector2> {
    size_t operator()(const Vector2& v) const
    {
        return hash<float>()(v.x) ^ hash<float>()(v.y);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_VEC2_H
