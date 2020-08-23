
#ifndef PETFOX
#define PETFOX
#include <Arduino.h>

#include <config.h>

//#include <WiFi.h>               //Do I need this?
#include <HTTPClient.h> //Remove Audio Lib error

#include "WatchManager.h"
#include "SoundManager.h"
#include "InputManager.h"

#include "apps/PetApp.h"
#include "apps/ClockApp.h"

WatchManager *manager = nullptr;
SoundManager *soundManager = nullptr;
InputManager *inputManager = nullptr;

PetApp *petApp = nullptr;
ClockApp *clockApp = nullptr;

void setup()
{
    //WiFi.mode(WIFI_OFF);
    //delay(500);
    Serial.begin(115200);

    manager = new WatchManager();
    manager->startup();

    soundManager = new SoundManager(manager);
    inputManager = new InputManager(manager);

    petApp = new PetApp(manager, soundManager);
    clockApp = new ClockApp(manager, soundManager);

    manager->addAppToList(petApp);
    manager->addAppToList(clockApp);

    manager->startApp(petApp);
}

void loop()
{
    //Pre Update
    inputManager->preUpdate();

    //Update
    manager->update();
    soundManager->update();
    inputManager->update();

    //Late Update
    inputManager->lateUpdate();
}

#endif
