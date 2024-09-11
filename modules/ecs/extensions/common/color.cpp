#include "color.h"

Color::Color() {
    this->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    property("color", &this->color);
}

Color::Color(const Vector4& color) {
    this->color = color;
    property("color", &this->color);
}

void Color::serialize(Archive& a) const {
    a << this->color;
}

void Color::deserialize(Archive& a) {
    a >> this->color;
}
