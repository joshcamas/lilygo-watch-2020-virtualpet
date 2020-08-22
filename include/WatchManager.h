#pragma once

#include "config.h"
#include "App.h"
#include <arduino-timer.h>

class App;

class WatchManager {

  private:
    App *currentApp = nullptr;
    bool vibrationEnabled = false;
    
    bool isSleeping = false;
    bool isDimmed = false;

    int sleepTimerStart;
    int dimTimerStart;

    const int dimTime = 3000;
    const int sleepTime = 6000;
    
    const int defaultBLBrightness = 255;
    const int dimBLBrightness = 10;
    
  public:

    TTGOClass *watch = nullptr;
    
    void startup();

    void initializeAccel();

    void startApp(App* app);

    void setBacklightBrightness(int value = 150);

    void enableVibration();

    void vibrate(int vibrateTime = 100);

    void resetSleepAndDimTimer();

    void loop();
    
    void enterSleep();

    void exitSleep();

    void startDim();

    void stopDim();
};

