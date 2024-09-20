#include "curve2d.h"

Curve2D::Curve2D() {
    points = List<Vector2>();
}

Curve2D::Curve2D(const List<Vector2>& points) {
    this->points = points;
}

Curve2D::Curve2D(const Curve2D& other) {
    points = other.points;
}

Curve2D& Curve2D::operator=(const Curve2D& other) {
    points = other.points;
    return *this;
}

Vector2 Curve2D::bezier(float t) const {
    // De Casteljau's algorithm
    List<Vector2> temp = points;
    while (temp.size() > 1) {
        for (int i = 0; i < temp.size() - 1; i++) {
            temp[i] = temp[i] + (temp[i + 1] - temp[i]) * t;
        }
        temp.pop_back();
    }
    return temp[0];
}

float Curve2D::linearLength() const {
    float length = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        length += (points[i + 1] - points[i]).length();
    }
    return length;
}

float Curve2D::bezierLength() const {
    float length = 0;
    Vector2 prev = bezier(0);
    for (float t = 0.01f; t <= 1.0f; t += 0.01f) {
        Vector2 next = bezier(t);
        length += (next - prev).length();
        prev = next;
    }
    return length;
}

void Curve2D::cast(const Ray2D& ray, Ray2D::Hit& hit) const {
    std::cout << "Not implemented" << std::endl;
}

void Curve2D::serialize(Archive &arc) const {
    arc << points;
}

void Curve2D::deserialize(Archive &arc) {
    arc >> points;
}
