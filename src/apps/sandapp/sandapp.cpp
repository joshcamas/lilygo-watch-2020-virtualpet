#include "config.h"
#include "apps/sandapp/sandapp.h"
#include "apps/sandapp/tiletype.h"
#include "apps/sandapp/tiletype_static.h"
#include "apps/sandapp/tiletype_sand.h"
#include "apps/sandapp/tiletype_water.h"

SandApp::SandApp(WatchManager *manager) : App(manager) {}

void SandApp::start()
{
    //Clear
    //lv_obj_clean(lv_scr_act());

    this->world = new World();
    this->worldUpdater = new WorldUpdater(this->world);

    TileType *airTile = new TileTypeStatic(0x000);
    TileType *blockTile = new TileTypeStatic(0xfff);
    TileType *sandTile = new TileTypeSand();
    TileType *waterTile = new TileTypeWater();

    this->world->addTileType(airTile);   //0
    this->world->addTileType(blockTile); //1
    this->world->addTileType(waterTile); //2
    this->world->addTileType(sandTile);  //3

    this->selectedTile = 2;

    this->worldDrawer = new WorldDrawer(this->world, this->manager);
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

    this->manager->watch->tft->fillScreen(TFT_BLACK);

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
    if (this->manager->watch->getTouch(x, y))
    {
        x = x / 4;
        y = y / 4;
        this->world->setTileIndexAtPosition(x, y, this->selectedTile);
        this->world->setTileIndexAtPosition(x + 1, y + 1, this->selectedTile);
        this->world->setTileIndexAtPosition(x + 1, y, this->selectedTile);
        this->world->setTileIndexAtPosition(x, y + 1, this->selectedTile);
    }

    //Update gravity
    uint8_t rotation = this->manager->watch->bma->direction();
    
    if (rotation == DIRECTION_TOP_EDGE)
        this->world->gravityDirection = DIR_UP;

    else if (rotation == DIRECTION_RIGHT_EDGE)
        this->world->gravityDirection = DIR_RIGHT;

    else if (rotation == DIRECTION_BOTTOM_EDGE)
        this->world->gravityDirection = DIR_DOWN;

    else if (rotation == DIRECTION_LEFT_EDGE)
        this->world->gravityDirection = DIR_LEFT;

    //Actually update world
    this->worldUpdater->update();
    this->worldDrawer->update();
}

bool SandApp::allowAutoSleep()
{
    return false;
}