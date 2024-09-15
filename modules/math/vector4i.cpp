#include "vector4i.h"

Vector4i::Vector4i()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4i::Vector4i(int v)
{
    x = v;
    y = v;
    z = v;
    w = v;
}

Vector4i::Vector4i(int x, int y, int z, int w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4i::Vector4i(const Vector4i& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

Vector4i Vector4i::operator+(const Vector4i& v) const
{
    return Vector4i(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4i Vector4i::operator-(const Vector4i& v) const
{
    return Vector4i(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4i Vector4i::operator*(const Vector4i& v) const
{
    return Vector4i(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4i Vector4i::operator/(const Vector4i& v) const
{
    return Vector4i(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4i Vector4i::operator*(float v) const
{
    return Vector4i(x * v, y * v, z * v, w * v);
}

Vector4i Vector4i::operator/(float v) const
{
    return Vector4i(x / v, y / v, z / v, w / v);
}

void Vector4i::operator+=(const Vector4i& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

void Vector4i::operator-=(const Vector4i& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

void Vector4i::operator*=(const Vector4i& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}

void Vector4i::operator/=(const Vector4i& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

void Vector4i::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
}

void Vector4i::operator/=(float v)
{
    x /= v;
    y /= v;
    z /= v;
    w /= v;
}

Vector4i& Vector4i::operator=(const Vector4i& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

bool Vector4i::operator==(const Vector4i& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

float Vector4i::dot(const Vector4i& v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

float Vector4i::length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}

Vector4i Vector4i::normalized() const
{
    float len = length();
    return Vector4i(x / len, y / len, z / len, w / len);
}

Vector4i Vector4i::lerp(const Vector4i& v, float t) const
{
    return *this + (v - *this) * t;
}

Vector4i Vector4i::slerp(const Vector4i& v, float t) const
{
    float dot = normalized().dot(v.normalized());
    dot = fmaxf(fminf(dot, 1), -1);
    float theta = acosf(dot) * t;
    Vector4i relative = (v - *this * dot).normalized();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector4i Vector4i::nlerp(const Vector4i& v, float t) const
{
    return lerp(v, t).normalized();
}

Vector4i Vector4i::cross(const Vector4i& v) const
{
    return Vector4i(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0);
}

Vector4i Vector4i::reflect(const Vector4i& normal) const
{
    return *this - normal * 2 * dot(normal);
}

Vector4i Vector4i::refract(const Vector4i& normal, float eta) const
{
    float dot = this->dot(normal);
    float k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f)
        return Vector4i(0);
    else
        return *this * eta - normal * (eta * dot + sqrtf(k));
}

void Vector4i::serialize(Archive& a) const
{
    a << x << y << z << w;
}

void Vector4i::deserialize(Archive& a)
{
    a >> x >> y >> z >> w;
}

std::array<int, 4> Vector4i::get() const
{
    return {x, y, z, w};
}