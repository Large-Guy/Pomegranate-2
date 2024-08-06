#include "matrix3x3.h"

#include <utility>

Matrix3x3::Matrix3x3()
{
    x = Vector3(1.0f, 0.0f, 0.0f);
    y = Vector3(0.0f, 1.0f, 0.0f);
    z = Vector3(0.0f, 0.0f, 1.0f);
}

Matrix3x3::Matrix3x3(const Vector3& x, const Vector3& y, const Vector3& z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Matrix3x3::Matrix3x3(float x0, float x1, float x2, float y0, float y1, float y2, float z0, float z1, float z2)
{
    x = Vector3(x0, x1, x2);
    y = Vector3(y0, y1, y2);
    z = Vector3(z0, z1, z2);
}

void Matrix3x3::serialize(Archive& a) const
{
    a << x << y << z;
}

void Matrix3x3::deserialize(Archive& a)
{
    a >> &x >> &y >> &z;
}

Matrix3x3 Matrix3x3::transpose() const {
    return Matrix3x3(x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z);
}

Matrix3x3 Matrix3x3::inverse() const {
    float det = x.x * y.y * z.z + x.y * y.z * z.x + x.z * y.x * z.y - x.z * y.y * z.x - x.y * y.x * z.z - x.x * y.z * z.y;
    if (det == 0) {
        return Matrix3x3();
    }
    float invDet = 1.0f / det;
    return Matrix3x3(
        (y.y * z.z - y.z * z.y) * invDet,
        (x.z * z.y - x.y * z.z) * invDet,
        (x.y * y.z - x.z * y.y) * invDet,
        (y.z * z.x - y.x * z.z) * invDet,
        (x.x * z.z - x.z * z.x) * invDet,
        (x.z * y.x - x.x * y.z) * invDet,
        (y.x * z.y - y.y * z.x) * invDet,
        (x.y * z.x - x.x * z.y) * invDet,
        (x.x * y.y - x.y * y.x) * invDet
    );
}

Matrix3x3 Matrix3x3::translate(Vector2 pos) const {
    return Matrix3x3(
        x.x, x.y, x.z,
        y.x, y.y, y.z,
        z.x + pos.x, z.y + pos.y, z.z
    );
}

Matrix3x3 Matrix3x3::scale(Vector2 scale) const {
    return Matrix3x3(
        x.x * scale.x, x.y * scale.y, x.z,
        y.x * scale.x, y.y * scale.y, y.z,
        z.x, z.y, z.z
    );
}

Matrix3x3 Matrix3x3::rotate(float angle) const {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Matrix3x3(
        x.x * c + y.x * s, x.y * c + y.y * s, x.z,
        -x.x * s + y.x * c, -x.y * s + y.y * c, y.z,
        z.x, z.y, z.z
    );
}

Matrix3x3 Matrix3x3::createTransform(Vector2 pos, Vector2 scale, float angle) {
    return Matrix3x3().scale(std::move(scale)).rotate(angle).translate(std::move(pos));
}
