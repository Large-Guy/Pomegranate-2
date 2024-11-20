#include "vector2.h"

Vector2::Vector2()
{
    x = 0;
    y = 0;
}

Vector2::Vector2(float v)
{
    x = v;
    y = v;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator/(const Vector2& v) const
{
    return Vector2(x / v.x, y / v.y);
}

Vector2 Vector2::operator*(float v) const
{
    return Vector2(x * v, y * v);
}

Vector2 Vector2::operator/(float v) const
{
    return Vector2(x / v, y / v);
}

void Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
}

void Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
}

void Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
}

void Vector2::operator/=(const Vector2& v)
{
    x /= v.x;
    y /= v.y;
}

void Vector2::operator*=(float v)
{
    x *= v;
    y *= v;
}

void Vector2::operator/=(float v)
{
    x /= v;
    y /= v;
}

bool Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}

float Vector2::dot(const Vector2 &v) const {
    return x * v.x + y * v.y;
}

float Vector2::length() const {
    return sqrtf(x * x + y * y);
}

Vector2 Vector2::normalize() const {
    float l = length();
    return Vector2(x / l, y / l);
}

Vector2 Vector2::lerp(const Vector2 &v, float t) const {
    return *this + (v - *this) * t;
}

Vector2 Vector2::slerp(const Vector2 &v, float t) const {
    float dot = normalize().dot(v.normalize());
    //Clamp it
    dot = fmaxf(fminf(dot, 1.0f), -1.0f);
    float theta = acosf(dot) * t;
    Vector2 relative = (v - *this * dot).normalize();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector2 Vector2::nlerp(const Vector2 &v, float t) const {
    return lerp(v, t).normalize();
}

Vector2 Vector2::cross(const Vector2 &v) const {
    return Vector2(y * v.x - x * v.y, x * v.y - y * v.x);
}

Vector2 Vector2::reflect(const Vector2 &normal) const {
    return *this - normal * 2 * dot(normal);
}

Vector2 Vector2::refract(const Vector2 &normal, float eta) const {
    float dot = this->dot(normal);
    float k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f) {
        return Vector2(0.0f);
    }
    return *this * eta - normal * (eta * dot + sqrtf(k));
}

Vector2 Vector2::rotate(float angle) const {
    float s = sinf(angle);
    float c = cosf(angle);
    return Vector2(x * c - y * s, x * s + y * c);
}

Vector2 Vector2::rotate(const Vector2 &pivot, float angle) const {
    return (*this - pivot).rotate(angle) + pivot;
}

void Vector2::serialize(Archive& a) const
{
    a << x << y;
}

void Vector2::deserialize(Archive& a)
{
    a >> x >> y;
}