#ifndef POMEGRANATEENGINE_VECTOR2I_H
#define POMEGRANATEENGINE_VECTOR2I_H
#include <core/serializable.h>
#include <cmath>

class Vector2i : public Serializable {
public:
    int x = 0, y = 0;
    Vector2i();
    Vector2i(int v);
    Vector2i(int x, int y);
    Vector2i(const Vector2i& other);

    Vector2i operator+(const Vector2i& v) const;
    Vector2i operator-(const Vector2i& v) const;
    Vector2i operator*(const Vector2i& v) const;
    Vector2i operator/(const Vector2i& v) const;
    Vector2i operator*(int v) const;
    Vector2i operator/(int v) const;
    void operator+=(const Vector2i& v);
    void operator-=(const Vector2i& v);
    void operator*=(const Vector2i& v);
    void operator/=(const Vector2i& v);
    void operator*=(int v);
    void operator/=(int v);
    Vector2i& operator=(const Vector2i& v);
    bool operator==(const Vector2i& v) const;

    [[nodiscard]] int dot(const Vector2i& v) const;
    [[nodiscard]] int length() const;
    [[nodiscard]] Vector2i normalized() const;
    [[nodiscard]] Vector2i lerp(const Vector2i& v, float t) const;
    [[nodiscard]] Vector2i slerp(const Vector2i& v, float t) const;
    [[nodiscard]] Vector2i nlerp(const Vector2i& v, float t) const;
    [[nodiscard]] Vector2i cross(const Vector2i& v) const;
    [[nodiscard]] Vector2i reflect(const Vector2i& normal) const;
    [[nodiscard]] Vector2i refract(const Vector2i& normal, float eta) const;
    [[nodiscard]] Vector2i rotate(float angle) const;
    [[nodiscard]] Vector2i rotate(const Vector2i& pivot, float angle) const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

template<>
struct std::hash<Vector2i> {
    size_t operator()(const Vector2i& v) const
    {
        return hash<int>()(v.x) ^ hash<int>()(v.y);
    }
};

#endif //POMEGRANATEENGINE_VECTOR2I_H
