
#include <Arduino.h>
#include "apps/sandapp/tiletype.h"
#include "apps/sandapp/worldupdater.h"

World::World() 
{
    this->tileTypeCount = 0;

    //Set values to "air"
    for(int i = 0; i < this->width * this->height; i++)
        this->tiles[i] = 0;

    //Set dirties to "true"
    for(int i = 0; i < this->width * this->height; i++)
        this->dirtyTiles[i] = true;

    for(int i = 0; i < 10; i++)
        this->tileTypes[i] = nullptr;
}


void World::clearDirty()
{
    for(int i = 0; i < this->width * this->height; i++)
        this->dirtyTiles[i] = false;

    this->isDirty = false;
}

void World::addTileType(TileType *tileType)
{
    this->tileTypes[this->tileTypeCount] = tileType;
    this->tileTypes[this->tileTypeCount]->setWorld(this,this->tileTypeCount);

    this->tileTypeCount += 1;
}

int World::getTileTypeCount() {
    return this->tileTypeCount;
}

int World::getTypeIndexFromTile(TileType *tileType)
{
    for (int i = 0; i < this->tileTypeCount; i++)
        if (this->tileTypes[i] == tileType)
            return i;

    return -1;
}

TileType *World::getTileTypeFromIndex(int index)
{
    if (index < 0 || index >= this->tileTypeCount)
        return nullptr;

    return this->tileTypes[index];
}

int World::getTileIndexAtPosition(int x, int y)
{
    if (x >= this->width || x < 0)
        return -1;

    if (y >= this->height || y < 0)
        return -1;

    //TODO: WTF???????
    if(y * this->height + x > this->height * this->width)
        return -1;

    return this->tiles[y * this->height + x];
}

bool World::isDirtyAtPosition(int x, int y)
{
    if (x >= this->width || x < 0)
        return false;

    if (y >= this->height || y < 0)
        return false;

    //TODO: WTF???????
    if(y * this->height + x > this->height * this->width)
        return false;

    return this->dirtyTiles[y * this->height + x];
}

void World::setTileIndexAtPosition(int x, int y, int index)
{
    if (x >= this->width || x < 0)
        return;

    if (y >= this->height || y < 0)
        return;

    if(index < 0 || index >= this->tileTypeCount)
        return;

    //TODO: WTF???????
    if(y * this->height + x > this->height * this->width)
        return;

    this->tiles[y * this->height + x] = index;
    this->dirtyTiles[y * this->height + x] = true;
    this->isDirty = true;
}

void World::drawVerticalLine(int x,int index) 
{
    for(int y = 0; y < this->height; y++)
        this->setTileIndexAtPosition(x,y,index);
}

void World::drawHorizontalLine(int y,int index)
{
    for(int x = 0; x < this->width; x++)
        this->setTileIndexAtPosition(x,y,index);
}

WorldUpdater::WorldUpdater(World* world) 
{
    this->world = world;
    this->updateDelay = 50;
}

void WorldUpdater::start()
{
}

void WorldUpdater::stop()
{
}

void WorldUpdater::update()
{
    //if(millis() - this->lastUpdate < this->updateDelay)
    //    return;

    for(int x = 0; x < this->world->width; x++)
    {
        for(int y = 0; y < this->world->height; y++) 
        {
            //TODO: Make this better
            if(this->checkerboardValue) 
            {
                if(!((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1)))
                    continue;
            }

            else 
            {
                if(!((x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0)))
                    continue;
            }

            int c = this->world->getTileIndexAtPosition(x,y);

            if(c < 0 || c >= this->world->getTileTypeCount())
                continue;

            TileType* tt = this->world->getTileTypeFromIndex(c);

            if(tt != nullptr)
                tt->updatePixel(x, y);
        }
    }

    this->checkerboardValue = !this->checkerboardValue;
    //this->lastUpdate = millis();
}