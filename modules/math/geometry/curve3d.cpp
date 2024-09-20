#include "curve3d.h"

Curve3D::Curve3D() {
    points = List<Vector3>();
}

Curve3D::Curve3D(const List<Vector3>& points) {
    this->points = points;
}

Curve3D::Curve3D(const Curve3D& other) {
    points = other.points;
}

Curve3D& Curve3D::operator=(const Curve3D& other) {
    points = other.points;
    return *this;
}

Vector3 Curve3D::bezier(float t) const {
    // De Casteljau's algorithm
    List<Vector3> temp = points;
    while (temp.size() > 1) {
        for (int i = 0; i < temp.size() - 1; i++) {
            temp[i] = temp[i] + (temp[i + 1] - temp[i]) * t;
        }
        temp.pop_back();
    }
    return temp[0];
}

float Curve3D::linearLength() const {
    float length = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        length += (points[i + 1] - points[i]).length();
    }
    return length;
}

float Curve3D::bezierLength() const {
    float length = 0;
    Vector3 prev = bezier(0);
    for (float t = 0.01f; t <= 1.0f; t += 0.01f) {
        Vector3 next = bezier(t);
        length += (next - prev).length();
        prev = next;
    }
    return length;
}

void Curve3D::serialize(Archive &arc) const {
    arc << points;
}

void Curve3D::deserialize(Archive &arc) {
    arc >> points;
}