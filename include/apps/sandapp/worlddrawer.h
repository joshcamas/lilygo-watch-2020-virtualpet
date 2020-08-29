#pragma once

#include <lvgl.h>
#include "apps/sandapp/worldupdater.h"
#include "WatchManager.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

class WorldDrawer 
{
private:
    World* world;
    int pixelWidth;
    int pixelHeight;
    WatchManager* manager;
public:
    WorldDrawer(World* world, WatchManager* manager);

    void start();
    void stop();
    void update();
};