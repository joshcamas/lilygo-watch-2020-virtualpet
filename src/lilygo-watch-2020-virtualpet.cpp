
#ifndef PETFOX
#define PETFOX
#include <Arduino.h>

#include <config.h>
#include <lvgl.h>

#include <WiFi.h>               //Do I need this?
#include <HTTPClient.h>         //Remove Audio Lib error

#include "WatchManager.h"
#include "SoundManager.h"

//Assets
#include "assets/sfx_fox_chirp.h"

LV_IMG_DECLARE(img_fox_sheet);


lv_obj_t* img1 = nullptr;
PlayingSound* sound = nullptr;
WatchManager* manager = nullptr;
SoundManager* soundManager = nullptr;

bool isPlaying = false;
bool down = false;

void setup()
{
    WiFi.mode(WIFI_OFF);
    delay(500);
    Serial.begin(115200);

    manager = new WatchManager();
    manager->startup();
    manager->enableVibration();

    soundManager = new SoundManager(manager);
    soundManager->enableAudio();

    img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_auto_size(img1, true);
    lv_img_set_src(img1, &img_fox_sheet);
    lv_img_set_antialias(img1, false);

    lv_obj_set_size(img1, 210, 210);
    lv_img_set_offset_x(img1, 0);

    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);

    //attach touch screen interrupt pin
    pinMode(TP_INT, INPUT);

    Serial.printf("Complete Startup\n");
}

void loop()
{
    manager->loop();
    soundManager->loop();

    if (isPlaying)
    {
        //Sound no longer playing
        if (!sound->isPlaying())
        {
            isPlaying = false;
            lv_img_set_offset_x(img1, 0);
        }
        //Sound still playing
        else
        {
            lv_img_set_offset_x(img1, 210);
        }

    }

    if (digitalRead(TP_INT) == LOW)
    {

        if (!isPlaying) {
            manager->vibrate(40);

            sound = soundManager->playWAVFromMem(sfx_fox_chirp, sizeof(sfx_fox_chirp));

            if (sound != nullptr)
                isPlaying = true;
        }

    }

}

#endif
