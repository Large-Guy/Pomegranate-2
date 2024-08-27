#include "color.h"

Color::Color() {
    this->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    property<Vector4>("color", &this->color);
}

Color::Color(const Vector4& color) {
    this->color = color;
    property<Vector4>("color", &this->color);
}
