
#include "WatchManager.h"
#include <lvgl.h>
#include <config.h>

void WatchManager::startup() {
  Serial.printf("===============[ Starting Watch Up ]===============\n");

  this->watch = TTGOClass::getWatch();
  this->watch->begin();

  this->resetSleepAndDimTimer();
  
  //Update brightness to default
  this->stopDim();

  //Setup power button
  pinMode(AXP202_INT, INPUT_PULLUP);
  this->watch->power->clearIRQ();

  //Setup accelerometer
  //this->initializeAccel();
}

void WatchManager::initializeAccel() {
  Acfg cfg;
  cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
  cfg.range = BMA4_ACCEL_RANGE_2G;
  cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
  cfg.perf_mode = BMA4_CONTINUOUS_MODE;

  this->watch->bma->accelConfig(cfg);
  this->watch->bma->enableAccel();
  this->watch->bma->enableFeature(BMA423_STEP_CNTR, false);
  this->watch->bma->enableFeature(BMA423_TILT, true);
  this->watch->bma->enableFeature(BMA423_WAKEUP, true);
  this->watch->bma->resetStepCounter();
  //this->watch->bma->enableTiltInterrupt();
  this->watch->bma->enableWakeupInterrupt();
}

void WatchManager::startApp(App* app) {
  if(this->currentApp != nullptr) {
    this->currentApp->stop();
    //delete this->currentApp;
  }

  this->currentApp = app;
  this->currentApp->start();
}

void WatchManager::setBacklightBrightness(int value) {
  this->watch->bl->adjust(value);      
}

void WatchManager::enableVibration() {
  this->vibrationEnabled = true;
  this->watch->motor_begin();
}

void WatchManager::vibrate(int vibrateTime) {
  if(!this->vibrationEnabled) {
    Serial.printf("ERROR: Cannot Vibrate if enableVibration() has never been executed\n");
    return;
  }

  this->watch->motor->onec(vibrateTime);
}

void WatchManager::resetSleepAndDimTimer() {

  this->sleepTimerStart = millis();
  this->dimTimerStart = millis();

  if(this->isDimmed)
    this->stopDim();
}

void WatchManager::loop() {
  
  //Start sleeping
  if(!this->isSleeping && ((millis() - this->sleepTimerStart) > this->sleepTime)) {
    this->enterSleep();
    return;
  }

  //Loop with sleeping flag enabled means device 
  //has been woken up
  if(this->isSleeping) {
    this->exitSleep();
  }

  //Start Dim
  if(!this->isDimmed && ((millis() - this->dimTimerStart) > this->dimTime))
    this->startDim();

  //Loop for LVGL
  lv_task_handler();

  if(this->currentApp != nullptr)
    this->currentApp->loop();

}

void WatchManager::enterSleep() {

  Serial.printf("Entering Sleep Mode\n");
  
  //Delay so the serial print can last haha
  delay(100);

  this->isSleeping = true;

  if(this->currentApp != nullptr)
    this->currentApp->startSleep();
    
  //Tilt sensor
  //esp_sleep_enable_ext1_wakeup(GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
  
  //Power Button
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_light_sleep_start();
}

void WatchManager::exitSleep() {
  
  Serial.printf("Exiting Sleep Mode\n");
  
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_EXT1);

  //Reset
  //this->watch->bma->reset();
  //this->initializeAccel();

  //Reset power interrupt
  this->watch->power->clearIRQ();

  this->isSleeping = false;

  if(this->currentApp != nullptr)
    this->currentApp->stopSleep();

  resetSleepAndDimTimer();
}

void WatchManager::startDim() {
  this->setBacklightBrightness(dimBLBrightness);
  this->isDimmed = true;
}

void WatchManager::stopDim() {
  this->setBacklightBrightness(defaultBLBrightness);
  this->isDimmed = false;
}
