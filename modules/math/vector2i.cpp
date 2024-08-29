#include "vector2i.h"

Vector2i::Vector2i()
{
    x = 0;
    y = 0;
}

Vector2i::Vector2i(int v)
{
    x = v;
    y = v;
}

Vector2i::Vector2i(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2i::Vector2i(const Vector2i& v)
{
    x = v.x;
    y = v.y;
}

Vector2i Vector2i::operator+(const Vector2i& v) const
{
    return Vector2i(x + v.x, y + v.y);
}

Vector2i Vector2i::operator-(const Vector2i& v) const
{
    return Vector2i(x - v.x, y - v.y);
}

Vector2i Vector2i::operator*(const Vector2i& v) const
{
    return Vector2i(x * v.x, y * v.y);
}

Vector2i Vector2i::operator/(const Vector2i& v) const
{
    return Vector2i(x / v.x, y / v.y);
}

Vector2i Vector2i::operator*(int v) const
{
    return Vector2i(x * v, y * v);
}

Vector2i Vector2i::operator/(int v) const
{
    return Vector2i(x / v, y / v);
}

void Vector2i::operator+=(const Vector2i& v)
{
    x += v.x;
    y += v.y;
}

void Vector2i::operator-=(const Vector2i& v)
{
    x -= v.x;
    y -= v.y;
}

void Vector2i::operator*=(const Vector2i& v)
{
    x *= v.x;
    y *= v.y;
}

void Vector2i::operator/=(const Vector2i& v)
{
    x /= v.x;
    y /= v.y;
}

void Vector2i::operator*=(int v)
{
    x *= v;
    y *= v;
}

void Vector2i::operator/=(int v)
{
    x /= v;
    y /= v;
}

Vector2i& Vector2i::operator=(const Vector2i& v)
{
    x = v.x;
    y = v.y;
    return *this;
}

bool Vector2i::operator==(const Vector2i& v) const
{
    return x == v.x && y == v.y;
}

int Vector2i::dot(const Vector2i &v) const {
    return x * v.x + y * v.y;
}

int Vector2i::length() const {
    return sqrtf(x * x + y * y);
}

Vector2i Vector2i::normalized() const {
    int l = length();
    return Vector2i(x / l, y / l);
}

Vector2i Vector2i::lerp(const Vector2i &v, float t) const {
    return *this + (v - *this) * t;
}

Vector2i Vector2i::slerp(const Vector2i &v, float t) const {
    int dot = normalized().dot(v.normalized());
    //Clamp
    dot = fmaxf(-1.0f, fminf(1.0f, dot));
    int theta = acosf(dot) * t;
    Vector2i relative = (v - *this * dot).normalized();
    return *this * cosf(theta) + relative * sinf(theta);
}

Vector2i Vector2i::nlerp(const Vector2i &v, float t) const {
    return lerp(v, t).normalized();
}

Vector2i Vector2i::cross(const Vector2i &v) const {
    return Vector2i(y * v.x - x * v.y, x * v.y - y * v.x);
}

Vector2i Vector2i::reflect(const Vector2i &normal) const {
    return *this - normal * 2 * dot(normal);
}

Vector2i Vector2i::refract(const Vector2i &normal, float eta) const {
    int dot = this->dot(normal);
    int k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f) {
        return Vector2i(0.0f);
    }
    return *this * eta - normal * (eta * dot + sqrtf(k));
}

Vector2i Vector2i::rotate(float angle) const {
    int s = sinf(angle);
    int c = cosf(angle);
    return Vector2i(x * c - y * s, x * s + y * c);
}

Vector2i Vector2i::rotate(const Vector2i &pivot, float angle) const {
    return (*this - pivot).rotate(angle) + pivot;
}

void Vector2i::serialize(Archive& a) const {
    a << x << y;
}

void Vector2i::deserialize(Archive& a) {
    a >> &x >> &y;
}