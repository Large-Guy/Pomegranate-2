#include "vector3i.h"

Vector3i::Vector3i()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3i::Vector3i(int v)
{
    x = v;
    y = v;
    z = v;
}

Vector3i::Vector3i(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3i::Vector3i(const Vector3i& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

Vector3i Vector3i::operator+(const Vector3i& v) const
{
    return Vector3i(x + v.x, y + v.y, z + v.z);
}

Vector3i Vector3i::operator-(const Vector3i& v) const
{
    return Vector3i(x - v.x, y - v.y, z - v.z);
}

Vector3i Vector3i::operator*(const Vector3i& v) const
{
    return Vector3i(x * v.x, y * v.y, z * v.z);
}

Vector3i Vector3i::operator/(const Vector3i& v) const
{
    return Vector3i(x / v.x, y / v.y, z / v.z);
}

Vector3i Vector3i::operator*(float v) const
{
    return Vector3i(x * v, y * v, z * v);
}

Vector3i Vector3i::operator/(float v) const
{
    return Vector3i(x / v, y / v, z / v);
}

void Vector3i::operator+=(const Vector3i& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector3i::operator-=(const Vector3i& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector3i::operator*=(const Vector3i& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

void Vector3i::operator/=(const Vector3i& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

void Vector3i::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
}

void Vector3i::operator/=(float v)
{
    x /= v;
    y /= v;
    z /= v;
}

Vector3i& Vector3i::operator=(const Vector3i& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

bool Vector3i::operator==(const Vector3i& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

float Vector3i::dot(const Vector3i& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

float Vector3i::length() const
{
    return sqrtf(x * x + y * y + z * z);
}

Vector3i Vector3i::normalized() const
{
    float l = length();
    return Vector3i(x / l, y / l, z / l);
}

Vector3i Vector3i::lerp(const Vector3i& v, float t) const
{
    return *this + (v - *this) * t;
}

Vector3i Vector3i::slerp(const Vector3i& v, float t) const
{
    float dot = normalized().dot(v.normalized());
    dot = std::clamp(dot, -1.0f, 1.0f);
    float theta = acosf(dot) * t;
    Vector3i relative = (v - *this * dot).normalized();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector3i Vector3i::nlerp(const Vector3i& v, float t) const
{
    return lerp(v, t).normalized();
}

Vector3i Vector3i::cross(const Vector3i& v) const
{
    return Vector3i(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3i Vector3i::reflect(const Vector3i& normal) const
{
    return *this - normal * 2 * dot(normal);
}

Vector3i Vector3i::refract(const Vector3i& normal, float eta) const
{
    float dot = this->dot(normal);
    float k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f)
        return Vector3i(0.0f);
    else
        return *this * eta - normal * (eta * dot + sqrtf(k));
}

void Vector3i::serialize(Archive& a) const
{
    a << x << y << z;
}

void Vector3i::deserialize(Archive& a)
{
    a >> &x >> &y >> &z;
}