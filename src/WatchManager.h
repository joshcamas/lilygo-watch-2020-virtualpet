#ifndef WATCHMANAGER_H
#define WATCHMANAGER_H


class WatchManager 
{
  private:
    bool vibrationEnabled = false;
    
  public:
    TTGOClass *watch = nullptr;
    
    WatchManager() 
    {
      
    }

    void startup() 
    {

      Serial.printf("Starting Up\n");
      this->watch = TTGOClass::getWatch();
      this->watch->begin();
      this->watch->lvgl_begin();
      this->watch->openBL();

      setBacklightBrightness(100);
      
    }
    void setBacklightBrightness(int value = 150)
    {
      this->watch->bl->adjust(value);      
    }
    
    void enableVibration()
    {
      this->vibrationEnabled = true;
      this->watch->motor_begin();
    }

    void vibrate(int vibrateTime = 100) 
    {
      if(!this->vibrationEnabled) {
        Serial.printf("ERROR: Cannot Vibrate if enableVibration() has never been executed\n");
        return;
      }

      this->watch->motor->onec(vibrateTime);
    }

    void loop()
    {
      //Loop for LVGL
      lv_task_handler();
    }
};

#endif
