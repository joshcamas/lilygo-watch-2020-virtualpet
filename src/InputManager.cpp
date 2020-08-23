
#include "InputManager.h"
#include "WatchManager.h"

InputManager::InputManager(WatchManager* manager) 
{
    this->manager = manager;
}

bool InputManager::isTouchPress()
{
    return this->isTouchPressValue && !this->isTouchUsed;
}

bool InputManager::isTouchRelease()
{
    return this->isTouchReleaseValue && !this->isTouchUsed;
}

bool InputManager::isTouchDown()
{
    return this->isTouchDownValue && !this->isTouchUsed;
}

int InputManager::getTouchX()
{
    return this->currentTouchX;
}

int InputManager::getTouchY()
{
    return this->currentTouchY;
}

void InputManager::useTouch()
{
    this->isTouchUsed = true;
}

void InputManager::preUpdate()
{
    this->isTouchUsed = false;
    
    int16_t x, y;
    bool newIsTouch = this->manager->watch->getTouch(x, y);

    //No press but still down
    if(newIsTouch && this->isTouchDownValue && this->isTouchPressValue)
        this->isTouchPressValue = false;

    //Press + down
    if(newIsTouch && !this->isTouchDownValue) 
    {
        this->isTouchPressValue = true;
        this->isTouchDownValue = true;
    } 

    //Released start
    if(!newIsTouch && this->isTouchDownValue) 
    {
        this->isTouchReleaseValue = true;
        this->isTouchDownValue = false;
    }
    
    //Released end
    else if(!newIsTouch && this->isTouchReleaseValue) 
    {
        this->isTouchReleaseValue = false;
    }

    this->currentTouchX = x;
    this->currentTouchY = y;
}
