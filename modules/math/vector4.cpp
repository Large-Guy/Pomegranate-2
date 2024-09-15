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

Vector4::Vector4(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator*(const Vector4& v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator/(const Vector4& v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4 Vector4::operator*(float v) const
{
    return Vector4(x * v, y * v, z * v, w * v);
}

Vector4 Vector4::operator/(float v) const
{
    return Vector4(x / v, y / v, z / v, w / v);
}

void Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

void Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

void Vector4::operator*=(const Vector4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}

void Vector4::operator/=(const Vector4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

void Vector4::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
}

void Vector4::operator/=(float v)
{
    x /= v;
    y /= v;
    z /= v;
    w /= v;
}

Vector4& Vector4::operator=(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

float Vector4::dot(const Vector4& v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

float Vector4::length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::normalized() const
{
    float len = length();
    return Vector4(x / len, y / len, z / len, w / len);
}

Vector4 Vector4::lerp(const Vector4& v, float t) const
{
    return *this + (v - *this) * t;
}

Vector4 Vector4::slerp(const Vector4& v, float t) const
{
    float dot = normalized().dot(v.normalized());
    dot = fmaxf(fminf(dot, 1), -1);
    float theta = acosf(dot) * t;
    Vector4 relative = (v - *this * dot).normalized();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector4 Vector4::nlerp(const Vector4& v, float t) const
{
    return lerp(v, t).normalized();
}

Vector4 Vector4::cross(const Vector4& v) const
{
    return Vector4(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0);
}

Vector4 Vector4::reflect(const Vector4& normal) const
{
    return *this - normal * 2 * dot(normal);
}

Vector4 Vector4::refract(const Vector4& normal, float eta) const
{
    float dot = this->dot(normal);
    float k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f)
        return Vector4(0);
    else
        return *this * eta - normal * (eta * dot + sqrtf(k));
}

void Vector4::serialize(Archive& a) const
{
    a << x << y << z << w;
}

void Vector4::deserialize(Archive& a)
{
    a >> x >> y >> z >> w;
}

std::array<float, 4> Vector4::get() const
{
    return {x, y, z, w};
}