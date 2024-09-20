#include "shape.h"

Shape::~Shape() {

}

bool Shape::contains(const Vector2& point) const {
    return false;
}

void Shape::cast(const Ray2D &ray, Hit2D &hit) const {

}

void Shape::serialize(Archive &a) const {

}

void Shape::deserialize(Archive &a) {

}
