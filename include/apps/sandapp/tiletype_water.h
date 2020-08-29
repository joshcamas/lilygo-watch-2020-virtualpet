#pragma once

#include <stdint.h>
#include "apps/sandapp/worldupdater.h"
#include "apps/sandapp/tiletype.h"

//Tile that falls and piles up
class TileTypeWater : public TileType
{
public:

    //Updates a world's pixel at a specific point
    void updatePixel(int x,int y, int frame) override
    { 

        bool grav = true;

        //down
        if(this->getPixelNeighbor(x,y,DIR_DOWN, grav) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,DIR_DOWN,this->selfIndex, grav);
            return;
        }

        int downA = (frame % 2 == 0) ? DIR_DOWNLEFT : DIR_DOWNRIGHT;
        int downB = (frame % 2 == 1) ? DIR_DOWNLEFT : DIR_DOWNRIGHT;

        if(this->getPixelNeighbor(x,y,downA,grav) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,downA,this->selfIndex,grav);
            return;
        }

        //down right
        if(this->getPixelNeighbor(x,y,downB,grav) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,downB,this->selfIndex,grav);
            return;
        }

        int sideA = (frame % 2 == 0) ? DIR_LEFT : DIR_RIGHT;
        int sideB = (frame % 2 == 1) ? DIR_LEFT : DIR_RIGHT;

        //left (if under)
        if(this->getPixelNeighbor(x,y,sideA, grav) == 0)
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,sideA,this->selfIndex, grav);
            return;
        }

        //right (if under)
        if(this->getPixelNeighbor(x,y,sideB, grav) == 0)
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->setPixelNeighbor(x,y,sideB,this->selfIndex, grav);
            return;
        }
    }

    //Returns a hex3 color of this tile type 
    uint32_t getPixelColorHex3() override { return 0x428df5; }
};
