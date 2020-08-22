
#ifndef PETFOX
#define PETFOX
#include <Arduino.h>

#include <config.h>

//#include <WiFi.h>               //Do I need this?
#include <HTTPClient.h>         //Remove Audio Lib error

#include "WatchManager.h"
#include "SoundManager.h"
#include "apps/PetApp.h"

WatchManager* manager = nullptr;
SoundManager* soundManager = nullptr;
PetApp* petApp = nullptr;

void setup()
{
    //WiFi.mode(WIFI_OFF);
    //delay(500);
    Serial.begin(115200);

    manager = new WatchManager();
    manager->startup();

    soundManager = new SoundManager(manager);

    petApp = new PetApp(manager,soundManager);
    manager->startApp(petApp);
}

void loop()
{
    manager->loop();
    soundManager->loop();
}

#endif
