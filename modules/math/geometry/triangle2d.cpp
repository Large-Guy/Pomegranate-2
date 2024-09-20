#include "triangle2d.h"

Triangle2D::Triangle2D() {
    a = Vector2();
    b = Vector2();
    c = Vector2();
}

Triangle2D::Triangle2D(Vector2 a, Vector2 b, Vector2 c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

Triangle2D::Triangle2D(float x1, float y1, float x2, float y2, float x3, float y3) {
    a = Vector2(x1, y1);
    b = Vector2(x2, y2);
    c = Vector2(x3, y3);
}

Triangle2D::Triangle2D(const Triangle2D &other) {
    a = other.a;
    b = other.b;
    c = other.c;
}

Triangle2D &Triangle2D::operator=(const Triangle2D &other) {
    a = other.a;
    b = other.b;
    c = other.c;
    return *this;
}

float Triangle2D::area() const {
    return std::abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0f);
}

Vector2 Triangle2D::center() const {
    return (a + b + c) / 3.0f;
}

bool Triangle2D::contains(const Vector2 &point) const {
    float area1 = std::abs((a.x * (b.y - point.y) + b.x * (point.y - a.y) + point.x * (a.y - b.y)) / 2.0f);
    float area2 = std::abs((a.x * (c.y - point.y) + c.x * (point.y - a.y) + point.x * (a.y - c.y)) / 2.0f);
    float area3 = std::abs((b.x * (c.y - point.y) + c.x * (point.y - b.y) + point.x * (b.y - c.y)) / 2.0f);
    float totalArea = area();
    return area1 + area2 + area3 == totalArea;
}

bool Triangle2D::intersects(const Triangle2D &other) const {
    return contains(other.a) || contains(other.b) || contains(other.c);
}

void Triangle2D::cast(const Ray2D &ray, Hit2D &hit) const {
    hit.hit = false;

    Segment2D ab = Segment2D(a, b);
    Segment2D bc = Segment2D(b, c);
    Segment2D ca = Segment2D(c, a);

    Hit2D abHit;
    Hit2D bcHit;
    Hit2D caHit;

    ab.cast(ray, abHit);
    bc.cast(ray, bcHit);
    ca.cast(ray, caHit);

    if (abHit.hit && (!hit.hit || abHit.distance < hit.distance)) {
        hit = abHit;
    }
}

void Triangle2D::serialize(Archive &arc) const {
    arc << a << b << c;
}

void Triangle2D::deserialize(Archive &arc) {
    arc >> a >> b >> c;
}
