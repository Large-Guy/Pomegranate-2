#ifndef POMEGRANATEENGINE_C_TILE_SET_H
#define POMEGRANATEENGINE_C_TILE_SET_H
#include <serializable.h>
#include "tile.h"
#include <component.h>

extern component_id COMPONENT_TILE_SET;

struct TileSet : public Serializable {
    std::vector<Tile> tiles = {};

    void addTile(const Tile& tile);
    bool inBounds(unsigned int index) const;
    Tile& getTile(unsigned int index);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_C_TILE_SET_H
