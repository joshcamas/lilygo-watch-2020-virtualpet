#include "config.h"
#include "apps/sandapp/sandapp.h"
#include "apps/sandapp/tiletype.h"
#include "apps/sandapp/tiletype_static.h"
#include "apps/sandapp/tiletype_sand.h"

SandApp::SandApp(WatchManager *manager) : App(manager) {}

void SandApp::start()
{
    //Clear
    lv_obj_clean(lv_scr_act());
    
    this->world = new World();
    this->worldUpdater = new WorldUpdater(this->world);

    TileType* airTile = new TileTypeStatic(0x000);
    TileType* blockTile = new TileTypeStatic(0xfff);
    TileType* sandTile = new TileTypeSand();

    this->world->addTileType(airTile);
    this->world->addTileType(blockTile);
    this->world->addTileType(sandTile);

    //Draw Box
    //this->world->drawVerticalLine(0,1);
    //this->world->drawVerticalLine(29,1);
    //this->world->drawHorizontalLine(0,1);
    //this->world->drawHorizontalLine(39,1);
    
    this->worldDrawer = new WorldDrawer(this->world);
    this->worldDrawer->start();

    //Set up accel

    Acfg cfg;
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    cfg.range = BMA4_ACCEL_RANGE_2G;
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;
    this->manager->watch->bma->accelConfig(cfg);
    this->manager->watch->bma->enableAccel();


    setCpuFrequencyMhz(240);
    
    Serial.printf("Completed Sand App Initialization\n");

}

void SandApp::stop()
{
    delete this->world;
    delete this->worldUpdater;
    delete this->worldDrawer;
}

void SandApp::loop()
{
    int16_t x, y;
    if(this->manager->watch->getTouch(x,y)) 
    {
        //Convert x to canvas 
        x = x/8;
        y = y/8;
        this->world->setTileIndexAtPosition(x,y,2);
    }

    //Update gravity
    uint8_t rotation = this->manager->watch->bma->direction();

    if(rotation == DIRECTION_TOP_EDGE)
        this->world->gravityDirection = 0;

    else if(rotation == DIRECTION_RIGHT_EDGE)
        this->world->gravityDirection = 1;
        
    else if(rotation == DIRECTION_BOTTOM_EDGE)
        this->world->gravityDirection = 2;

    else if(rotation == DIRECTION_LEFT_EDGE)
        this->world->gravityDirection = 3;


    //Actually update world
    this->worldUpdater->update();
    this->worldDrawer->update();
}

bool SandApp::allowAutoSleep()
{
    return false;
}