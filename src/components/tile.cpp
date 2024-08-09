#include "tile.h"

Tile::Tile() {
    rect = Rect();
    origin = Vector2();
    texture = nullptr;
}

Tile::Tile(Rect rect, Vector2 origin, Texture2D* texture) {
    this->rect = rect;
    this->origin = origin;
    this->texture = texture;
}

void Tile::serialize(Archive &a) const {
    a << rect << origin << texture;
}

void Tile::deserialize(Archive &a) {
    a >> &rect >> &origin >> texture;
}