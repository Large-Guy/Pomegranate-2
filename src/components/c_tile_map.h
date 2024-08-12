#ifndef POMEGRANATEENGINE_TILEMAP_H
#define POMEGRANATEENGINE_TILEMAP_H
#include <serializable.h>
#include <texture.h>
#include <rect.h>
#include <model2d.h>
#include <shader.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <thread>
#include <vector2i.h>
#include <component.h>

typedef unsigned int tile_id;
typedef Vector2i chunk_id;

#define CHUNK_SIZE 64

extern component_id COMPONENT_TILE_MAP;

struct TileMap : public Serializable {
    std::unordered_map<chunk_id, std::unordered_map<tile_id,std::vector<Vector2i>>> tiles = {};
    std::unordered_map<chunk_id, std::unordered_map<Vector2i, tile_id>> getTiles = {};
    std::unordered_map<chunk_id, std::unordered_map<tile_id, Model2D*>> models = {};
    std::unordered_map<chunk_id, std::unordered_set<tile_id>> dirty = {};
    Shader* shader = nullptr;
    bool modelNeedsBuilding = false;

    TileMap();
    void setTile(Vector2i pos, unsigned int tile);
    void removeTile(Vector2i pos);
    tile_id getTile(Vector2i pos);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

#endif //POMEGRANATEENGINE_TILEMAP_H
