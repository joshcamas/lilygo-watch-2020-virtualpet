#pragma once

#include <stdint.h>
#include "apps/sandapp/worldupdater.h"

class World;

class TileType
{

protected:
    World *world;
    int selfIndex;

public:
    void setWorld(World *world, int selfIndex);

    //Updates a world's pixel at a specific point
    virtual void updatePixel(int x, int y, int frame);

    //Returns a hex3 color of this tile type
    virtual uint32_t getPixelColorHex3();

    void transformNeighbor(int x, int y, int direction, bool transformByGravity, int &tx,int &ty);

    int getPixelNeighbor(int x, int y, int direction, bool transformByGravity = false);
    
    void setPixelNeighbor(int x, int y, int direction, int value, bool transformByGravity = false);
};
