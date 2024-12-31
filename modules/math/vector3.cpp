#include "vector3.h"
#include "vector2.h"
#include "vector2i.h"
#include "vector3i.h"
#include "vector4.h"
#include "vector4i.h"

const Vector3 Vector3::zero = Vector3(0);
const Vector3 Vector3::one = Vector3(1);
const Vector3 Vector3::up = Vector3(0, 1, 0);
const Vector3 Vector3::down = Vector3(0, -1, 0);
const Vector3 Vector3::left = Vector3(-1, 0, 0);
const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::forward = Vector3(0, 0, 1);
const Vector3 Vector3::back = Vector3(0, 0, -1);

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

Vector3 Vector3::normalize() const
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
    float dot = normalize().dot(v.normalize());
    //Clamp
    dot = fmaxf(fminf(dot, 1.0f), -1.0f);
    float theta = acosf(dot) * t;
    Vector3 relative = (v - *this * dot).normalize();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector3 Vector3::nlerp(const Vector3& v, float t) const
{
    return lerp(v, t).normalize();
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

Vector3 Vector3::rotate(float angle) const
{
    float s = sinf(angle);
    float c = cosf(angle);
    return Vector3(x * c - y * s, x * s + y * c, z);
}

Vector3 Vector3::rotate(const Vector3& rotation) const
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float a = rotation.x;
    float b = rotation.y;
    float c = rotation.z;
    float cosA = cosf(a);
    float sinA = sinf(a);
    float cosB = cosf(b);
    float sinB = sinf(b);
    float cosC = cosf(c);
    float sinC = sinf(c);
    float x1 = x * (cosB * cosC) + y * (cosB * sinC) + z * (-sinB);
    float y1 = x * (sinA * sinB * cosC - cosA * sinC) + y * (sinA * sinB * sinC + cosA * cosC) + z * (sinA * cosB);
    float z1 = x * (cosA * sinB * cosC + sinA * sinC) + y * (cosA * sinB * sinC - sinA * cosC) + z * (cosA * cosB);
    return Vector3(x1, y1, z1);
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

Vector3::operator Vector2() const {
    return {x, y};
}

Vector3::operator Vector2i() const {
    return {(int)x, (int)y};
}

Vector3::operator Vector3i() const {
    return {(int)x, (int)y, (int)z};
}

Vector3::operator Vector4() const {
    return {x, y, z, 0};
}

Vector3::operator Vector4i() const {
    return {(int)x, (int)y, (int)z, 0};
}
