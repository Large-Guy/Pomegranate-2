#ifndef POMEGRANATEENGINE_TILEMAP_H
#define POMEGRANATEENGINE_TILEMAP_H
#include <serializable.h>
#include <texture.h>
#include <rect.h>
#include <model2d.h>
#include <shader.h>
#include <unordered_map>
#include <memory>
#include <thread>

typedef unsigned int tile_id;

#define TILE_MAP 1
struct TileMap : public Serializable {
    int width = 0, height = 0;
    tile_id* tiles = nullptr;
    std::unordered_map<Texture2D*, Model2D*> models = {};
    bool changes = false;
    Shader* shader = nullptr;
    std::thread* generationThread = nullptr;
    bool generationRunning = false;
    bool modelNeedsBuilding = false;

    TileMap();
    void resize(int width, int height);
    bool inBounds(int x, int y);
    void setTile(int x, int y, unsigned int tile);
    tile_id getTile(int x, int y);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

#endif //POMEGRANATEENGINE_TILEMAP_H
