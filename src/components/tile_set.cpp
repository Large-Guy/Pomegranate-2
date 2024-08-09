#include "tile_set.h"

void TileSet::addTile(const Tile &tile) {
    tiles.push_back(tile);
}

bool TileSet::inBounds(unsigned int index) const {
    return index < tiles.size();
}

Tile& TileSet::getTile(unsigned int index) {
    return tiles[index];
}

void TileSet::serialize(Archive &a) const {
    a << tiles.size();
    for(auto& tile : tiles) {
        a << tile;
    }
}

void TileSet::deserialize(Archive &a) {
    int size;
    a >> &size;
    for(int i = 0; i < size; i++) {
        Tile tile;
        a >> &tile;
        tiles.push_back(tile);
    }
}