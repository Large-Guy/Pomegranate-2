#ifndef POMEGRANATEENGINE_TILE_SET_H
#define POMEGRANATEENGINE_TILE_SET_H
#include <serializable.h>
#include "tile.h"

#define TILE_SET 0
struct TileSet : public Serializable {
    std::vector<Tile> tiles = {};

    void addTile(const Tile& tile);
    bool inBounds(unsigned int index) const;
    Tile& getTile(unsigned int index);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_TILE_SET_H
