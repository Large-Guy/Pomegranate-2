#ifndef POMEGRANATEENGINE_TILE_H
#define POMEGRANATEENGINE_TILE_H
#include <serializable.h>
#include <rect.h>
#include <texture.h>
#include <memory>

struct Tile : public Serializable
{
    Rect rect = Rect();
    Vector2 origin = Vector2();
    Texture2D* texture = nullptr;

    Tile();
    Tile(Rect rect, Vector2 origin, Texture2D* texture = nullptr);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

#endif //POMEGRANATEENGINE_TILE_H
