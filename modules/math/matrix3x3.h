#ifndef POMEGRANATEENGINE_MATRIX3X3_H
#define POMEGRANATEENGINE_MATRIX3X3_H
#include <vector3.h>
#include <vector2.h>
#include <cmath>

struct Matrix3x3 : public Serializable {
    Vector3 x = Vector3();
    Vector3 y = Vector3();
    Vector3 z = Vector3();

    Matrix3x3();
    Matrix3x3(const Vector3& x, const Vector3& y, const Vector3& z);
    Matrix3x3(float x0, float x1, float x2, float y0, float y1, float y2, float z0, float z1, float z2);

    [[nodiscard]] Matrix3x3 transpose() const;
    [[nodiscard]] Matrix3x3 inverse() const;
    [[nodiscard]] Matrix3x3 translate(Vector2 pos) const;
    [[nodiscard]] Matrix3x3 scale(Vector2 scale) const;
    [[nodiscard]] Matrix3x3 rotate(float angle) const;

    static Matrix3x3 createTransform(Vector2 pos, Vector2 scale, float angle);
    static Matrix3x3 createOrthographic(float left, float right, float bottom, float top);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_MATRIX3X3_H
