#include "matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    x = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    y = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    z = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    w = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4::Matrix4x4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Matrix4x4::Matrix4x4(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, float z0, float z1, float z2, float z3, float w0, float w1, float w2, float w3)
{
    x = Vector4(x0, x1, x2, x3);
    y = Vector4(y0, y1, y2, y3);
    z = Vector4(z0, z1, z2, z3);
    w = Vector4(w0, w1, w2, w3);
}

void Matrix4x4::serialize(Archive& a) const
{
    a << x << y << z << w;
}

void Matrix4x4::deserialize(Archive& a)
{
    a >> &x >> &y >> &z >> &w;
}

Matrix4x4 Matrix4x4::transpose() const {
    return Matrix4x4(x.x, y.x, z.x, w.x, x.y, y.y, z.y, w.y, x.z, y.z, z.z, w.z, x.w, y.w, z.w, w.w);
}

Matrix4x4 Matrix4x4::inverse() const {
    float det = x.x * y.y * z.z * w.w + x.y * y.z * z.w * w.x + x.z * y.w * z.x * w.y + x.w * y.x * z.y * w.z
              - x.w * y.z * z.y * w.x - x.z * y.x * z.w * w.y - x.y * y.w * z.x * w.z - x.x * y.y * z.z * w.w;
    if (det == 0) {
        return Matrix4x4();
    }
    float invDet = 1.0f / det;
    return Matrix4x4(
        (y.y * z.z * w.w + y.z * z.w * w.y + y.w * z.y * w.z - y.w * z.z * w.y - y.z * z.y * w.w - y.y * z.w * w.z) * invDet,
        (x.w * z.z * w.y + x.z * z.w * w.w + x.y * z.y * w.z - x.y * z.z * w.w - x.z * z.y * w.w - x.w * z.w * w.z) * invDet,
        (x.y * y.z * w.w + x.z * y.w * w.y + x.w * y.y * w.z - x.w * y.z * w.y - x.z * y.y * w.w - x.y * y.w * w.z) * invDet,
        (x.w * y.z * z.y + x.z * y.y * z.w + x.y * y.w * z.z - x.y * y.z * z.w - x.z * y.w * z.y - x.w * y.y * z.z) * invDet,
        (y.w * z.x * w.y + y.z * z.w * w.x + y.x * z.y * w.z - y.x * z.w * w.y - y.z * z.y * w.x - y.w * z.z * w.z) * invDet,
        (x.x * z.w * w.y + x.w * z.y * w.x + x.y * z.x * w.w - x.y * z.w * w.x - x.w * z.x * w.y - x.x * z.y * w.w) * invDet,
        (x.w * y.x * w.y + x.x * y.w * w.z + x.y * y.x * w.w - x.y * y.w * w.x - x.x * y.x * w.w - x.w * y.x * w.y) * invDet,
        (x.x * y.z * z.y + x.z * y.y * z.x + x.y * y.x * z.z - x.y * y.z * z.x - x.z * y.x * z.y - x.x * y.y * z.z) * invDet,
        (y.z * z.x * w.x + y.x * z.w * w.z + y.w * z.z * w.x - y.w * z.x * w.z - y.x * z.z * w.x - y.z * z.w * w.x) * invDet,
        (x.x * z.z * w.x + x.z * z.x * w.w + x.w * z.z * w.x - x.w * z.x * w.z - x.z * z.z * w.x - x.x * z.w * w.z) * invDet,
        (x.w * y.z * w.x + x.z * y.w * w.x + x.x * y.z * w.w - x.x * y.w * w.z - x.z * y.z * w.x - x.w * y.x * w.w) * invDet,
        (x.z * y.x * z.w + x.w * y.z * z.x + x.x * y.w * z.z - x.x * y.z * z.w - x.w * y.x * z.z - x.z * y.w * z.x) * invDet,
        (y.y * z.x * w.w + y.w * z.w * w.y + y.x * z.y * w.w - y.x * z.w * w.y - y.w * z.y * w.w - y.y * z.w * w.x) * invDet,
        (x.w * z.x * w.y + x.x * z.w * w.y + x.y * z.x * w.w - x.y * z.w * w.x - x.x * z.x * w.w - x.w * z.y * w.y) * invDet,
        (x.x * y.w * w.y + x.w * y.y * w.x + x.y * y.x * w.w - x.y * y.w * w.x - x.w * y.x * w.y - x.x * y.y * w.w) * invDet,
        (x.y * y.z * z.x + x.z * y.x * z.y + x.x * y.y * z.z - x.x * y.z * z.y - x.z * y.y * z.x - x.y * y.x * z.z) * invDet
    );
}

Matrix4x4 Matrix4x4::translate(Vector3 pos) const {
    return Matrix4x4(
        x.x, x.y, x.z, x.w,
        y.x, y.y, y.z, y.w,
        z.x, z.y, z.z, z.w,
        w.x + pos.x, w.y + pos.y, w.z + pos.z, w.w
    );
}

Matrix4x4 Matrix4x4::scale(Vector3 scale) const {
    return Matrix4x4(
        x.x * scale.x, x.y * scale.y, x.z * scale.z, x.w,
        y.x * scale.x, y.y * scale.y, y.z * scale.z, y.w,
        z.x * scale.x, z.y * scale.y, z.z * scale.z, z.w,
        w.x, w.y, w.z, w.w
    );
}

Matrix4x4 Matrix4x4::rotateX(float angle) const {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Matrix4x4(
        x.x, x.y, x.z, x.w,
        y.x * c - z.x * s, y.y * c - z.y * s, y.z * c - z.z * s, y.w * c - z.w * s,
        y.x * s + z.x * c, y.y * s + z.y * c, y.z * s + z.z * c, y.w * s + z.w * c,
        w.x, w.y, w.z, w.w
    );
}

Matrix4x4 Matrix4x4::rotateY(float angle) const {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Matrix4x4(
        x.x * c + z.x * s, x.y * c + z.y * s, x.z * c + z.z * s, x.w * c + z.w * s,
        y.x, y.y, y.z, y.w,
        -x.x * s + z.x * c, -x.y * s + z.y * c, -x.z * s + z.z * c, -x.w * s + z.w * c,
        w.x, w.y, w.z, w.w
    );
}

Matrix4x4 Matrix4x4::rotateZ(float angle) const {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Matrix4x4(
        x.x * c - y.x * s, x.y * c - y.y * s, x.z * c - y.z * s, x.w * c - y.w * s,
        x.x * s + y.x * c, x.y * s + y.y * c, x.z * s + y.z * c, x.w * s + y.w * c,
        z.x, z.y, z.z, z.w,
        w.x, w.y, w.z, w.w
    );
}

Matrix4x4 Matrix4x4::createTransform(Vector3 pos, Vector3 scale, Vector3 rotation) {
    return Matrix4x4().scale(scale).rotateX(rotation.x).rotateY(rotation.y).rotateZ(rotation.z).translate(pos);
}
