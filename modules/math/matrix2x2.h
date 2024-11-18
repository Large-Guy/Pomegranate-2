#ifndef POMEGRANATE_ENGINE_MATH_MATRIX2X2_H
#define POMEGRANATE_ENGINE_MATH_MATRIX2X2_H
#include"vector2.h"

struct Matrix2x2 {
    Vector2 x = Vector2();
    Vector2 y = Vector2();

    Matrix2x2();
    Matrix2x2(const Vector2& x, const Vector2& y);
    Matrix2x2(float x0, float x1, float y0, float y1);

    [[nodiscard]] Matrix2x2 transpose() const;
    [[nodiscard]] Matrix2x2 inverse() const;
    [[nodiscard]] Matrix2x2 dot(const Matrix2x2& m) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};

template<>
struct std::hash<Matrix2x2> {
    size_t operator()(const Matrix2x2& m) const
    {
        return hash<Vector2>()(m.x) ^ hash<Vector2>()(m.y);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_MATRIX2X2_H
