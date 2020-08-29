#pragma once

#include <stdint.h>
#include "apps/sandapp/worldupdater.h"
#include "apps/sandapp/tiletype.h"

//Tile that falls and piles up
class TileTypeSand : public TileType
{
public:

    //Updates a world's pixel at a specific point
    void updatePixel(int x,int y, int frame) override
    { 
        //down
        if(this->getPixelNeighbor(x,y,DIR_DOWN,true) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,DIR_DOWN,this->selfIndex,true);
            return;
        }

        //down left
        if(frame % 2 == 0 && this->getPixelNeighbor(x,y,DIR_DOWNLEFT,true) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,DIR_DOWNLEFT,this->selfIndex,true);
            return;
        }

        //down right
        if(frame % 2 == 1 && this->getPixelNeighbor(x,y,DIR_DOWNRIGHT,true) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,DIR_DOWNRIGHT,this->selfIndex,true);
            return;
        }

    }

    //Returns a hex3 color of this tile type 
    uint32_t getPixelColorHex3() override { return 0xf0f032; }
};
