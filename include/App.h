#pragma once

#include "WatchManager.h"

class WatchManager;

class App {

    protected:
    WatchManager *manager;

    public:
    App(WatchManager *manager);

    virtual void start();

    virtual void stop();

    virtual void loop();

    void startSleep();

    void stopSleep();

};
