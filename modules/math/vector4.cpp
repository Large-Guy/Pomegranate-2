#include "vector4.h"

Vector4::Vector4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4::Vector4(float v)
{
    x = v;
    y = v;
    z = v;
    w = v;
}

Vector4::Vector4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4 Vector4::operator+(const Vector4& v)
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator-(const Vector4& v)
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator*(const Vector4& v)
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator/(const Vector4& v)
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

void Vector4::serialize(Archive& a) const
{
    a << x << y << z << w;
}

void Vector4::deserialize(Archive& a)
{
    a >> &x >> &y >> &z >> &w;
}