#pragma once

#include "WatchManager.h"

class InputManager
{
private:
    WatchManager* manager;
    bool isTouchPressValue;
    bool isTouchDownValue;
    bool isTouchReleaseValue;
    bool isTouchUsed;
    int currentTouchX;
    int currentTouchY;
public:
    InputManager(WatchManager* manager);
    bool isTouchPress();
    bool isTouchRelease();
    bool isTouchDown();
    int getTouchX();
    int getTouchY();
    void useTouch();
    void preUpdate();
    void update();
    void lateUpdate();
};