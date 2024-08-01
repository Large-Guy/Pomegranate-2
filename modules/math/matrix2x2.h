#ifndef POMEGRANATEENGINE_MATRIX2X2_H
#define POMEGRANATEENGINE_MATRIX2X2_H
#include<vector2.h>

struct Matrix2x2 : public Serializable {
    Vector2 x = Vector2();
    Vector2 y = Vector2();

    Matrix2x2();
    Matrix2x2(const Vector2& x, const Vector2& y);
    Matrix2x2(float x0, float x1, float y0, float y1);

    [[nodiscard]] Matrix2x2 transpose() const;
    [[nodiscard]] Matrix2x2 inverse() const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_MATRIX2X2_H
