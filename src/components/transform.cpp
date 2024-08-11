#include "transform.h"

Transform2D::Transform2D()
{
    position = Vector2();
    scale = Vector2(1.0f);
    rotation = 0.0f;
}

Transform2D::Transform2D(const Vector2& position, const Vector2& scale, float rotation)
{
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
}

Matrix3x3 Transform2D::getMatrix()
{
    if(position == _position && scale == _scale && rotation == _rotation) {
        return _matrix;
    }
    else
    {
        _position = position;
        _scale = scale;
        _rotation = rotation;
        _matrix = Matrix3x3::createTransform(position, scale, rotation);
        return _matrix;
    }
}

void Transform2D::serialize(Archive& a) const
{
    a << position << scale << rotation;
}

void Transform2D::deserialize(Archive& a)
{
    a >> &position >> &scale >> &rotation;
}