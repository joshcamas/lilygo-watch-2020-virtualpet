#pragma once

#include "config.h"
#include "App.h"
#include <arduino-timer.h>

class App;

class WatchManager {

  private:
    App *currentApp = nullptr;
    int currentAppIndex = 0;

    App *appList[10];

    bool vibrationEnabled = false;
    
    bool isSleeping = false;
    bool isDimmed = false;
    bool touchDown = false;
    int sleepTimerStart;
    int dimTimerStart;

    const int dimTime = 5000;
    const int sleepTime = 7000;
    
    const int defaultBLBrightness = 255;
    const int dimBLBrightness = 10;

    void powerButtonSwitchApp();

    void initializePower();

    void initializeAccel();

    void enterSleep();

    void exitSleep();

    void startDim();

    void stopDim();

    bool checkAppSwitch();
  public:

    TTGOClass *watch = nullptr;
    
    void startup();
    
    bool allowTouchInput();

    int addAppToList(App* app);

    int getAppIndex(App* app);

    bool startApp(App* app);
    
    bool startAppIndex(int appIndex);

    void setBacklightBrightness(int value = 150);

    void enableVibration();

    void disableVibration();

    void vibrate(int vibrateTime = 100);

    void resetSleepAndDimTimer();

    void update();
    
};

