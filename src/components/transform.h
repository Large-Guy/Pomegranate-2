#ifndef POMEGRANATEENGINE_TRANSFORM_H
#define POMEGRANATEENGINE_TRANSFORM_H
#include <serializable.h>
#include <vector2.h>
#include <matrix3x3.h>

#define TRANSFORM_2D 3
struct Transform2D : public Serializable {
private:
    Matrix3x3 _matrix = Matrix3x3();
    Vector2 _position = Vector2();
    Vector2 _scale = Vector2(1.0f);
    float _rotation = 0.0f;
public:
    Vector2 position = Vector2();
    Vector2 scale = Vector2(1.0f);
    float rotation = 0.0f;

    Transform2D();
    Transform2D(const Vector2& position, const Vector2& scale, float rotation);

    Matrix3x3 getMatrix();

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

#endif //POMEGRANATEENGINE_TRANSFORM_H
