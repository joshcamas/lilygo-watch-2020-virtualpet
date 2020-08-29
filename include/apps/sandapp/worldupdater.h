#pragma once

#include "apps/sandapp/tiletype.h"

class TileType;

#define DIR_NONE 0
#define DIR_UP 1
#define DIR_RIGHT 2
#define DIR_DOWN 3
#define DIR_LEFT 4
#define DIR_UPRIGHT 5
#define DIR_UPLEFT 6
#define DIR_DOWNRIGHT 7
#define DIR_DOWNLEFT 8

class World
{
private:
    //All tiletypes defined
    TileType* tileTypes[10];
    int tileTypeCount = 0;

public:

    //Note hardcoded values for screen size
    const static int width = 60;
    const static int height = 60;

    bool isDirty = true;
    int tiles[width*height];
    bool dirtyTiles[width*height];

    World();

    int gravityDirection = DIR_DOWN;
    
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
    bool enableCheckerboard = true;
    int frame;

public:
    WorldUpdater(World* world);

    void start();

    void stop();

    void update();
};