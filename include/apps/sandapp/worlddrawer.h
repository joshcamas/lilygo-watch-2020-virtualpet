#pragma once

#include <lvgl.h>
#include "apps/sandapp/worldupdater.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

class WorldDrawer 
{
private:
    lv_obj_t *canvas;
    World* world;
    int pixelWidth;
    int pixelHeight;
    lv_color_t cbuffer [LV_CANVAS_BUF_SIZE_INDEXED_4BIT(SCREEN_WIDTH,SCREEN_HEIGHT)];
public:
    WorldDrawer(World* world);

    void start();
    void stop();
    void update();
};