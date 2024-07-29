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

Vector2 Vector2::operator+(const Vector2& v)
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v)
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const Vector2& v)
{
    return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator/(const Vector2& v)
{
    return Vector2(x / v.x, y / v.y);
}

void Vector2::serialize(Archive& a) const
{
    a << x << y;
}

void Vector2::deserialize(Archive& a)
{
    a >> &x >> &y;
}

Vector2::operator std::string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}
