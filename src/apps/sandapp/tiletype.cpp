#include "apps/sandapp/worldupdater.h"
#include "apps/sandapp/tiletype.h"
#include <Arduino.h>

void TileType::setWorld(World *world, int selfIndex)
{
    this->world = world;
    this->selfIndex = selfIndex;
}

//Updates a world's pixel at a specific point
void TileType::updatePixel(int x, int y, int frame) {}

//Returns a hex3 color of this tile type
uint32_t TileType::getPixelColorHex3() { return 0x9ef; }

void TileType::transformNeighbor(int x, int y, int direction, bool transformByGravity, int &tx,int &ty)
{
    int dx = 0;
    int dy = 0;

    if (direction == DIR_UP || direction == DIR_UPLEFT || direction == DIR_UPRIGHT)
        dy = -1;

    if (direction == DIR_DOWN || direction == DIR_DOWNLEFT || direction == DIR_DOWNRIGHT)
        dy = 1;

    if (direction == DIR_LEFT || direction == DIR_UPLEFT || direction == DIR_DOWNLEFT)
        dx = 1;

    if (direction == DIR_RIGHT || direction == DIR_UPRIGHT || direction == DIR_DOWNRIGHT)
        dx = -1;
    
    if(transformByGravity) 
    {
        if(this->world->gravityDirection == DIR_UP) 
        {
            dy = -dy;
            dx = -dx;
        }
        else if(this->world->gravityDirection == DIR_RIGHT)
        {
            int old = dy;
            dy = -dx;
            dx = -old;
        }
        else if(this->world->gravityDirection == DIR_LEFT)
        {
            int old = dy;
            dy = dx;
            dx = old;
        }
    }
    
    tx = x + dx;
    ty = y + dy;
}

int TileType::getPixelNeighbor(int x, int y, int direction, bool transformByGravity)
{
    int tX,tY;
    this->transformNeighbor(x,y,direction,transformByGravity,tX,tY);
    return this->world->getTileIndexAtPosition(tX,tY);
}
    
void TileType::setPixelNeighbor(int x, int y, int direction, int value, bool transformByGravity)
{
    int tX,tY;
    this->transformNeighbor(x,y,direction,transformByGravity,tX,tY);
    this->world->setTileIndexAtPosition(tX,tY,value);
}