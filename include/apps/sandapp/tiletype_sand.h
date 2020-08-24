#pragma once

#include <stdint.h>
#include "apps/sandapp/worldupdater.h"
#include "apps/sandapp/tiletype.h"

//Tile that falls and piles up
class TileTypeSand : public TileType
{
public:

    //Updates a world's pixel at a specific point
    void updatePixel(int x,int y) override
    { 
        int down = 0;

        //Going towards top
        if(this->world->gravityDirection == 0)
            down = -1;

        //Going towards bottom
        else if(this->world->gravityDirection == 2)
            down = 1;

        //Terrible code incomming
        if(down == 1 || down == -1) 
        {
            if(this->world->getTileIndexAtPosition(x,y+down) == 0) 
            {
                this->world->setTileIndexAtPosition(x,y,0);
                this->world->setTileIndexAtPosition(x,y+down,this->selfIndex);
                return;
            }
            if(this->world->getTileIndexAtPosition(x+1,y+down) == 0) 
            {
                this->world->setTileIndexAtPosition(x,y,0);
                this->world->setTileIndexAtPosition(x+1,y+down,this->selfIndex);
                return;
            }
            
            if(this->world->getTileIndexAtPosition(x-1,y+down) == 0) 
            {
                this->world->setTileIndexAtPosition(x,y,0);
                this->world->setTileIndexAtPosition(x-1,y+down,this->selfIndex);
                return;
            }
            return;
        }

        //Going towards right
        if(this->world->gravityDirection == 3)
            down = 1;

        //Going towards left
        else if(this->world->gravityDirection == 1)
            down = -1;

        if(this->world->getTileIndexAtPosition(x+down,y) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->world->setTileIndexAtPosition(x+down,y,this->selfIndex);
            return;
        }
        if(this->world->getTileIndexAtPosition(x+down,y+1) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->world->setTileIndexAtPosition(x+down,y+1,this->selfIndex);
            return;
        }
        
        if(this->world->getTileIndexAtPosition(x+down,y-1) == 0) 
        {
            this->world->setTileIndexAtPosition(x,y,0);
            this->world->setTileIndexAtPosition(x+down,y-1,this->selfIndex);
            return;
        }

    }

    //Returns a hex3 color of this tile type 
    uint32_t getPixelColorHex3() override { return 0xff0; }
};
