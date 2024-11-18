#include "matrix2x2.h"

Matrix2x2::Matrix2x2()
{
    x = Vector2(1.0f, 0.0f);
    y = Vector2(0.0f, 1.0f);
}

Matrix2x2::Matrix2x2(const Vector2& x, const Vector2& y)
{
    this->x = x;
    this->y = y;
}

Matrix2x2::Matrix2x2(float x0, float x1, float y0, float y1)
{
    x = Vector2(x0, x1);
    y = Vector2(y0, y1);
}

void Matrix2x2::serialize(Archive& a) const
{
    a << x << y;
}

void Matrix2x2::deserialize(Archive& a)
{
    a >> x >> y;
}

Matrix2x2 Matrix2x2::transpose() const {
    return Matrix2x2(x.x, y.x, x.y, y.y);
}

Matrix2x2 Matrix2x2::inverse() const {
    float det = x.x * y.y - x.y * y.x;
    if (det == 0) {
        return Matrix2x2();
    }
    float invDet = 1.0f / det;
    return Matrix2x2(y.y * invDet, -x.y * invDet, -y.x * invDet, x.x * invDet);
}

Matrix2x2 Matrix2x2::dot(const Matrix2x2& m) const {
    return Matrix2x2(
        x.x * m.x.x + x.y * m.y.x,
        x.x * m.x.y + x.y * m.y.y,
        y.x * m.x.x + y.y * m.y.x,
        y.x * m.x.y + y.y * m.y.y
    );
}

Matrix2x2 Matrix2x2::operator*(const Matrix2x2& m) const {
    return dot(m);
}

Vector2 Matrix2x2::operator*(const Vector2& v) const {
    return Vector2(x.x * v.x + x.y * v.y, y.x * v.x + y.y * v.y);
}

Matrix2x2 Matrix2x2::operator*(float v) const {
    return Matrix2x2(x.x * v, x.y * v, y.x * v, y.y * v);
}

bool Matrix2x2::operator==(const Matrix2x2& m) const {
    return x == m.x && y == m.y;
}