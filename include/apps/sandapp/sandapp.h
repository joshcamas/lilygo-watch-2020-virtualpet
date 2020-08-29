#pragma once

#include "App.h"
#include "apps/sandapp/worldupdater.h"
#include "apps/sandapp/worlddrawer.h"

class SandApp : public App
{
private:
    World* world;
    WorldUpdater* worldUpdater;
    WorldDrawer* worldDrawer;
    int selectedTile = 1;
    
public:
    SandApp(WatchManager *manager);

    void start() override;

    void stop() override;

    void loop() override;

    bool allowAutoSleep() override;
};