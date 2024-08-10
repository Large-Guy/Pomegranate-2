#include "tile_map.h"

TileMap::TileMap() {
    width = 0;
    height = 0;
    tiles = nullptr;
    models = std::unordered_map<Texture2D*, Model2D*>();
    changes = false;
    shader = nullptr;
}

void TileMap::resize(int width, int height) {
    this->width = width;
    this->height = height;
    if(tiles != nullptr) delete[] tiles;
    tiles = new tile_id[width * height];
    changes = true;
}

bool TileMap::inBounds(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void TileMap::setTile(int x, int y, tile_id tile) {
    if(!inBounds(x, y)) return;
    tiles[y * width + x] = tile;
    changes = true;
}

tile_id TileMap::getTile(int x, int y) {
    if(!inBounds(x, y)) return 0;
    return tiles[y * width + x];
}

void TileMap::serialize(Archive &a) const {
    a << width << height;
    for(int i = 0; i < width * height; i++) {
        a << tiles[i];
    }
}

void TileMap::deserialize(Archive &a) {
    a >> &width >> &height;
    if(tiles != nullptr) delete[] tiles;
    tiles = new tile_id[width * height];
    for(int i = 0; i < width * height; i++) {
        a >> &tiles[i];
    }
}