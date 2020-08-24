#include "apps/sandapp/worlddrawer.h"
#include "apps/sandapp/worldupdater.h"
#include <Arduino.h>

WorldDrawer::WorldDrawer(World* world)
{
    this->world = world;
}

void WorldDrawer::start() 
{
    this->pixelWidth = SCREEN_WIDTH / World::width;
    this->pixelHeight = SCREEN_HEIGHT / World::height;

    this->canvas = lv_canvas_create(lv_scr_act(), NULL);
    lv_canvas_set_buffer(this->canvas, this->cbuffer, SCREEN_WIDTH, SCREEN_HEIGHT, LV_IMG_CF_INDEXED_4BIT);
    lv_obj_align(this->canvas, NULL, LV_ALIGN_CENTER, 0, 0);

    if(this->world->getTileTypeCount() == 0) 
        Serial.printf("ERROR: No tile types defined, so palette cannot be created");

    for(int i = 0; i < this->world->getTileTypeCount(); i++) {
        auto color = this->world->getTileTypeFromIndex(i)->getPixelColorHex3();
        lv_canvas_set_palette(this->canvas, i, lv_color_hex3(color));
    }

    //Temp
    lv_color_t c; c.full = 0;
    lv_canvas_fill_bg(this->canvas, c, LV_OPA_COVER);
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

            lv_color_t c; 
            c.full = index;

            //Set pixels in area
            for(int xx = 0; xx < this->pixelWidth; xx++)
                for(int yy = 0; yy < this->pixelHeight; yy++)
                    lv_canvas_set_px(this->canvas, x * this->pixelWidth + xx, y * this->pixelHeight + yy, c);
        }
    }

    this->world->clearDirty();
}