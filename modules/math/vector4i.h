#ifndef POMEGRANATEENGINE_VECTOR4I_H
#define POMEGRANATEENGINE_VECTOR4I_H
#include <core/serializable.h>
#include <cmath>

struct Vector4i : public Serializable {
    int x = 0, y = 0, z = 0, w = 0;
    Vector4i();
    Vector4i(int v);
    Vector4i(int x, int y, int z, int w = 0.0f);
    Vector4i(const Vector4i& other);

    Vector4i operator+(const Vector4i& v) const;
    Vector4i operator-(const Vector4i& v) const;
    Vector4i operator*(const Vector4i& v) const;
    Vector4i operator/(const Vector4i& v) const;
    Vector4i operator*(float v) const;
    Vector4i operator/(float v) const;
    void operator+=(const Vector4i& v);
    void operator-=(const Vector4i& v);
    void operator*=(const Vector4i& v);
    void operator/=(const Vector4i& v);
    void operator*=(float v);
    void operator/=(float v);
    Vector4i& operator=(const Vector4i& v);
    bool operator==(const Vector4i& v) const;

    [[nodiscard]] float dot(const Vector4i& v) const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector4i normalized() const;
    [[nodiscard]] Vector4i lerp(const Vector4i& v, float t) const;
    [[nodiscard]] Vector4i slerp(const Vector4i& v, float t) const;
    [[nodiscard]] Vector4i nlerp(const Vector4i& v, float t) const;
    [[nodiscard]] Vector4i cross(const Vector4i& v) const;
    [[nodiscard]] Vector4i reflect(const Vector4i& normal) const;
    [[nodiscard]] Vector4i refract(const Vector4i& normal, float eta) const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;

};


#endif //POMEGRANATEENGINE_VECTOR4I_H