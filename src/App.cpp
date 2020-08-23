
#include "App.h"
#include "WatchManager.h"

App::App(WatchManager *manager) {
        this->manager = manager;
}

void App::start() {}

void App::stop() {}

void App::loop() {}

void App::startSleep() {
        this->manager->vibrate(60);
        
        //Delay to allow vibration to happen
        delay(100);

        this->manager->watch->displaySleep();
        this->manager->watch->closeBL();
        //this->manager->watch->powerOff();
    }

void App::stopSleep() {
        this->manager->watch->displayWakeup();
        this->manager->watch->openBL();
        //this->manager->vibrate(60);
        //this->chirp();
    }

