#include "vector3.h"

Vector3::Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(float v)
{
    x = v;
    y = v;
    z = v;
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::operator+(const Vector3& v)
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v)
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(const Vector3& v)
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator/(const Vector3& v)
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

void Vector3::serialize(Archive& a) const
{
    a << x << y << z;
}

void Vector3::deserialize(Archive& a)
{
    a >> &x >> &y >> &z;
}