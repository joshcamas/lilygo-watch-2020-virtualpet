#pragma once

#include <stdint.h>
#include "apps/sandapp/worldupdater.h"

class World;

class TileType
{
    
protected:
    World* world;
    int selfIndex;

public:
    void setWorld(World* world,int selfIndex)
    {
        this->world = world;
        this->selfIndex = selfIndex;
    }

    //Updates a world's pixel at a specific point
    virtual void updatePixel(int x,int y) { }

    //Returns a hex3 color of this tile type
    virtual uint32_t getPixelColorHex3() { return 0x9ef; }
};
