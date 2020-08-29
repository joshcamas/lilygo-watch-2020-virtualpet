#include "apps/sandapp/worlddrawer.h"
#include "apps/sandapp/worldupdater.h"
#include "WatchManager.h"
#include <Arduino.h>

WorldDrawer::WorldDrawer(World* world,WatchManager* manager)
{
    this->world = world;
    this->manager = manager;
}

void WorldDrawer::start() 
{
    this->pixelWidth = SCREEN_WIDTH / World::width;
    this->pixelHeight = SCREEN_HEIGHT / World::height;

}

void WorldDrawer::stop() 
{

}

void WorldDrawer::update() 
{
    if(!this->world->isDirty)
        return;
    
    for(int x = 0; x < this->world->width; x++)
    {
        for(int y = 0; y < this->world->height; y++) 
        {
            if(!this->world->isDirtyAtPosition(x,y))
                continue;

            int index = this->world->getTileIndexAtPosition(x,y);
            TileType* tiletype = this->world->getTileTypeFromIndex(index);

            if(tiletype == nullptr)
                continue;

            this->manager->watch->tft->fillRect(x * this->pixelWidth,y * this->pixelHeight, this->pixelWidth, this->pixelHeight,tiletype->getPixelColorHex3());
        }
    }

    this->world->clearDirty();
}