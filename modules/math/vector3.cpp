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

Vector3::Vector3(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(const Vector3& v) const
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator/(const Vector3& v) const
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3 Vector3::operator*(float v) const
{
    return Vector3(x * v, y * v, z * v);
}

Vector3 Vector3::operator/(float v) const
{
    return Vector3(x / v, y / v, z / v);
}

void Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector3::operator*=(const Vector3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

void Vector3::operator/=(const Vector3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

void Vector3::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
}

void Vector3::operator/=(float v)
{
    x /= v;
    y /= v;
    z /= v;
}

Vector3& Vector3::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

bool Vector3::operator==(const Vector3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

float Vector3::dot(const Vector3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

float Vector3::length() const
{
    return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const
{
    float l = length();
    return Vector3(x / l, y / l, z / l);
}

Vector3 Vector3::lerp(const Vector3& v, float t) const
{
    return *this + (v - *this) * t;
}

Vector3 Vector3::slerp(const Vector3& v, float t) const
{
    float dot = normalized().dot(v.normalized());
    //Clamp
    dot = fmaxf(fminf(dot, 1.0f), -1.0f);
    float theta = acosf(dot) * t;
    Vector3 relative = (v - *this * dot).normalized();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector3 Vector3::nlerp(const Vector3& v, float t) const
{
    return lerp(v, t).normalized();
}

Vector3 Vector3::cross(const Vector3& v) const
{
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3 Vector3::reflect(const Vector3& normal) const
{
    return *this - normal * 2 * dot(normal);
}

Vector3 Vector3::refract(const Vector3& normal, float eta) const
{
    float dot = this->dot(normal);
    float k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f)
        return Vector3(0.0f);
    else
        return *this * eta - normal * (eta * dot + sqrtf(k));
}

void Vector3::serialize(Archive& a) const
{
    a << x << y << z;
}

void Vector3::deserialize(Archive& a)
{
    a >> x >> y >> z;
}

std::array<float, 3> Vector3::get() const
{
    return {x, y, z};
}