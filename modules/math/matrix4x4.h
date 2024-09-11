#ifndef POMEGRANATE_ENGINE_MATH_MATRIX4X4_H
#define POMEGRANATE_ENGINE_MATH_MATRIX4X4_H
#include "vector4.h"
#include "vector3.h"
#include <cmath>

struct Matrix4x4 {
    Vector4 x = Vector4();
    Vector4 y = Vector4();
    Vector4 z = Vector4();
    Vector4 w = Vector4();

    Matrix4x4();
    Matrix4x4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w);
    Matrix4x4(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, float z0, float z1, float z2, float z3, float w0, float w1, float w2, float w3);

    [[nodiscard]] Matrix4x4 transpose() const;
    [[nodiscard]] Matrix4x4 inverse() const;
    [[nodiscard]] Matrix4x4 translate(Vector3 pos) const;
    [[nodiscard]] Matrix4x4 scale(Vector3 scale) const;
    [[nodiscard]] Matrix4x4 rotateX(float angle) const;
    [[nodiscard]] Matrix4x4 rotateY(float angle) const;
    [[nodiscard]] Matrix4x4 rotateZ(float angle) const;

    static Matrix4x4 createTransform(Vector3 pos, Vector3 scale, Vector3 rotation);

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};

template<>
struct std::hash<Matrix4x4> {
    size_t operator()(const Matrix4x4& m) const
    {
        return hash<Vector4>()(m.x) ^ hash<Vector4>()(m.y) ^ hash<Vector4>()(m.z) ^ hash<Vector4>()(m.w);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_MATRIX4X4_H
