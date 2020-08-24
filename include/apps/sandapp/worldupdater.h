#pragma once

#include "apps/sandapp/tiletype.h"

class TileType;

class World
{
private:
    //All tiletypes defined
    TileType* tileTypes[10];
    int tileTypeCount = 0;
public:

    //Note hardcoded values for screen size
    const static int width = 30;
    const static int height = 30;

    bool isDirty = true;
    int tiles[width*height];
    bool dirtyTiles[width*height];

    World();

    int gravityDirection = 2;
    
    void addTileType(TileType *tileType);
    int getTypeIndexFromTile(TileType *tile);
    TileType *getTileTypeFromIndex(int index);
    int getTileTypeCount();

    int getTileIndexAtPosition(int x, int y);
    void setTileIndexAtPosition(int x, int y, int index);

    void drawVerticalLine(int x,int index);
    void drawHorizontalLine(int y,int index);
    
    bool isDirtyAtPosition(int x, int y);
    void clearDirty();
};

class WorldUpdater
{
private:
    World* world;
    int updateDelay = 100;
    int lastUpdate;
    bool checkerboardValue = false;

public:
    WorldUpdater(World* world);

    void start();

    void stop();

    void update();
};