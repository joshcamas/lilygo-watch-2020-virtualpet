#pragma once

#include <stdint.h>
#include "apps/sandapp/worldupdater.h"
#include "apps/sandapp/tiletype.h"

//Simple non-moving tile that can easily have custom
//color defined
class TileTypeStatic : public TileType
{
private:
    uint32_t color;

public:
    TileTypeStatic(uint32_t color) 
    {
        this->color = color;
    }

    //Returns a hex3 color of this tile type 
    uint32_t getPixelColorHex3() override { return this->color; }
};
