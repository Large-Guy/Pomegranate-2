#include "vector4.h"

const Vector4 Vector4::zero = Vector4(0);
const Vector4 Vector4::one = Vector4(1);
const Vector4 Vector4::up = Vector4(0, 1, 0, 0);
const Vector4 Vector4::down = Vector4(0, -1, 0, 0);
const Vector4 Vector4::left = Vector4(-1, 0, 0, 0);
const Vector4 Vector4::right = Vector4(1, 0, 0, 0);
const Vector4 Vector4::forward = Vector4(0, 0, 1, 0);
const Vector4 Vector4::back = Vector4(0, 0, -1, 0);

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

Vector4 Vector4::normalize() const
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
    float dot = normalize().dot(v.normalize());
    dot = fmaxf(fminf(dot, 1), -1);
    float theta = acosf(dot) * t;
    Vector4 relative = (v - *this * dot).normalize();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector4 Vector4::nlerp(const Vector4& v, float t) const
{
    return lerp(v, t).normalize();
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