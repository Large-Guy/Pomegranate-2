#ifndef POMEGRANATE_ENGINE_MATH_VECTOR3I_H
#define POMEGRANATE_ENGINE_MATH_VECTOR3I_H
#include <core/serializable.h>
#include <cmath>

struct Vector3i : public Serializable {
    int x = 0, y = 0, z = 0;
    Vector3i();
    Vector3i(int v);
    Vector3i(int x, int y, int z = 0.0f);
    Vector3i(const Vector3i& other);

    Vector3i operator+(const Vector3i& v) const;
    Vector3i operator-(const Vector3i& v) const;
    Vector3i operator*(const Vector3i& v) const;
    Vector3i operator/(const Vector3i& v) const;
    Vector3i operator*(float v) const;
    Vector3i operator/(float v) const;
    void operator+=(const Vector3i& v);
    void operator-=(const Vector3i& v);
    void operator*=(const Vector3i& v);
    void operator/=(const Vector3i& v);
    void operator*=(float v);
    void operator/=(float v);
    Vector3i& operator=(const Vector3i& v);
    bool operator==(const Vector3i& v) const;

    [[nodiscard]] float dot(const Vector3i& v) const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector3i normalized() const;
    [[nodiscard]] Vector3i lerp(const Vector3i& v, float t) const;
    [[nodiscard]] Vector3i slerp(const Vector3i& v, float t) const;
    [[nodiscard]] Vector3i nlerp(const Vector3i& v, float t) const;
    [[nodiscard]] Vector3i cross(const Vector3i& v) const;
    [[nodiscard]] Vector3i reflect(const Vector3i& normal) const;
    [[nodiscard]] Vector3i refract(const Vector3i& normal, float eta) const;
    //Vector3i rotate(int angle) const;
    //Vector3i rotate(const Vector3i& pivot, int angle) const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

template<>
struct std::hash<Vector3i> {
    size_t operator()(const Vector3i& v) const
    {
        return hash<int>()(v.x) ^ hash<int>()(v.y) ^ hash<int>()(v.z);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_VECTOR3I_H
