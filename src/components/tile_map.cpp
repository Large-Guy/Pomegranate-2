#include "tile_map.h"

component_id COMPONENT_TILE_MAP = 0;

TileMap::TileMap() {
    tiles = std::unordered_map<chunk_id, std::unordered_map<tile_id,std::vector<Vector2i>>>();
    getTiles = std::unordered_map<chunk_id, std::unordered_map<Vector2i, tile_id>>();
    models = std::unordered_map<chunk_id, std::unordered_map<tile_id, Model2D*>>();
    dirty = std::unordered_map<chunk_id, std::unordered_set<tile_id>>();
    shader = nullptr;
}

void TileMap::setTile(Vector2i pos, tile_id tile) {
    Vector2i chunk = pos / Vector2i(CHUNK_SIZE,CHUNK_SIZE);
    Vector2i local = pos - chunk * Vector2i(CHUNK_SIZE,CHUNK_SIZE);
    if(getTiles[chunk].count(local) > 0) {
        removeTile(pos);
    }
    tiles[chunk][tile].push_back(local);
    getTiles[chunk][local] = tile;
    dirty[chunk].insert(tile);
}

void TileMap::removeTile(Vector2i pos) {
    Vector2i chunk = pos / Vector2i(CHUNK_SIZE,CHUNK_SIZE);
    Vector2i local = pos - chunk * Vector2i(CHUNK_SIZE,CHUNK_SIZE);
    tile_id tile = getTiles[chunk][local];
    tiles[chunk][tile].erase(std::remove(tiles[chunk][tile].begin(),tiles[chunk][tile].end(),local),tiles[chunk][tile].end());
    getTiles[chunk].erase(local);
    dirty[chunk].insert(tile);
}

tile_id TileMap::getTile(Vector2i pos) {
    Vector2i chunk = pos / Vector2i(CHUNK_SIZE,CHUNK_SIZE);
    Vector2i local = pos - chunk * Vector2i(CHUNK_SIZE,CHUNK_SIZE);
    return getTiles[chunk][local];
}

void TileMap::serialize(Archive &a) const {

}

void TileMap::deserialize(Archive &a) {

}