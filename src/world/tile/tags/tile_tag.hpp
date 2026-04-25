#pragma once

class Tile;

struct TileTag
{
    TileTag(Tile* myTile);

    virtual ~TileTag();

    Tile* myTile;
};