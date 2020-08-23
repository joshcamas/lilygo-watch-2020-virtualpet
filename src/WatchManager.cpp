
#include "WatchManager.h"
#include <lvgl.h>
#include <config.h>

void WatchManager::startup()
{
    Serial.printf("===============[ Starting Watch Up ]===============\n");

    this->watch = TTGOClass::getWatch();
    this->watch->begin();

    //Startup LVGL
    this->watch->lvgl_begin();
    this->watch->openBL();

    //Start vibration
    this->enableVibration();

    this->resetSleepAndDimTimer();

    //Update brightness to default
    this->stopDim();

    //Setup power button
    this->initializePower();

    //Build array
    for (int i = 0; i < 10; i++)
        this->appList[i] = nullptr;

    //Setup accelerometer
    //this->initializeAccel();
}

void WatchManager::initializePower()
{

    this->watch->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    this->watch->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);

    this->watch->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    this->watch->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    this->watch->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
    this->watch->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

    pinMode(AXP202_INT, INPUT_PULLUP);
    this->watch->power->clearIRQ();
}

void WatchManager::initializeAccel()
{
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

int WatchManager::addAppToList(App *app)
{
    for (int i = 0; i < 10; i++)
    {
        //Found empty slot
        if (this->appList[i] == nullptr)
        {
            this->appList[i] = app;
            return i;
        }
    }

    return -1;
}

int WatchManager::getAppIndex(App *app)
{
    for (int i = 0; i < 10; i++)
    {
        if (this->appList[i] == app)
            return i;
    }
    return -1;
}

bool WatchManager::startApp(App *app)
{
    return this - startAppIndex(this->getAppIndex(app));
}

bool WatchManager::startAppIndex(int index)
{

    if (index < 0 || index >= 10)
        return false;

    if (this->appList[index] == nullptr)
        return false;

    if (this->currentApp != nullptr)
        this->currentApp->stop();

    this->currentApp = appList[index];
    this->currentAppIndex = index;

    this->currentApp->start();

    return true;
}

bool WatchManager::allowTouchInput() 
{
    return !this->touchDown;
}
    
void WatchManager::setBacklightBrightness(int value) 
{
    this->watch->bl->adjust(value);
}

void WatchManager::enableVibration()
{
    this->vibrationEnabled = true;
    this->watch->motor_begin();
}

void WatchManager::disableVibration()
{
    this->vibrationEnabled = false;
}

void WatchManager::vibrate(int vibrateTime)
{
    if (!this->vibrationEnabled)
        return;

    this->watch->motor->onec(vibrateTime);
}

void WatchManager::resetSleepAndDimTimer()
{

    this->sleepTimerStart = millis();
    this->dimTimerStart = millis();

    this->stopDim();
}

bool WatchManager::checkAppSwitch()
{

    int16_t x, y;
    if (this->watch->getTouch(x, y))
    {
        if (y > 170 && this->touchDown == false)
        {
            this->touchDown = true;
            return true;
        }
    }
    else
    {
        this->touchDown = false;
    }

    return false;
}

void WatchManager::powerButtonSwitchApp()
{
    this->vibrate(10);

    this->currentAppIndex += 1;

    if (this->currentAppIndex >= 10 || this->appList[this->currentAppIndex] == nullptr)
        this->currentAppIndex = 0;

    this->startAppIndex(this->currentAppIndex);
}

//Device loop
void WatchManager::update()
{
    //Start sleeping
    if (!this->isSleeping && ((millis() - this->sleepTimerStart) > this->sleepTime))
    {
        this->enterSleep();
        return;
    }

    //Loop with sleeping flag enabled means device
    //has been woken up
    if (this->isSleeping)
    {
        this->exitSleep();
    }

    //Start Dim
    if (!this->isDimmed && ((millis() - this->dimTimerStart) > this->dimTime))
        this->startDim();

    //Loop for LVGL
    lv_task_handler();

    //Detect app switch

    if (this->checkAppSwitch()) 
    {
        
        resetSleepAndDimTimer();
        this->powerButtonSwitchApp();
        return;
    }

    if (this->currentApp != nullptr)
        this->currentApp->loop();

    /*
    //Detect power press
    this->watch->power->readIRQ();

    if (this->watch->power->isPEKShortPressIRQ())
    {
        this->resetSleepAndDimTimer();
        this->powerButtonSwitchApp();
        Serial.printf("Button Press\n");
    }
    this->watch->power->clearIRQ();*/

}

//Enters light sleep mode
void WatchManager::enterSleep()
{

    Serial.printf("Entering Sleep Mode\n");

    //Disable vibration due to issue
    this->disableVibration();

    //Reset power interrupt
    this->watch->power->clearIRQ();

    //Delay so the serial print can last haha
    delay(100);

    this->isSleeping = true;

    if (this->currentApp != nullptr)
        this->currentApp->startSleep();

    //Tilt sensor
    //esp_sleep_enable_ext1_wakeup(GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);

    //Power Button
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_light_sleep_start();
}

//Disables light sleep mode
void WatchManager::exitSleep()
{

    Serial.printf("Exiting Sleep Mode\n");

    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_EXT1);
    
    //Re-enable vibration
    this->enableVibration();
    
    //Reset
    //this->watch->bma->reset();
    //this->initializeAccel();

    //Reset power interrupt
    this->watch->power->clearIRQ();

    this->isSleeping = false;

    if (this->currentApp != nullptr)
        this->currentApp->stopSleep();

    resetSleepAndDimTimer();
}

//Dims the screen's backlight
void WatchManager::startDim()
{
    this->setBacklightBrightness(dimBLBrightness);
    this->isDimmed = true;
}

//Undims the screen's backlight
void WatchManager::stopDim()
{
    this->setBacklightBrightness(defaultBLBrightness);
    this->isDimmed = false;
}
