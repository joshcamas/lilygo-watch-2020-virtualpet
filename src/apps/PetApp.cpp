
#include <lvgl.h>

#include "apps/PetApp.h"
#include "WatchManager.h"
#include "SoundManager.h"
#include "App.h"

//Assets
#include "assets/sfx_fox_chirp.h"

LV_IMG_DECLARE(img_fox_sheet);


PetApp::PetApp(WatchManager* manager,SoundManager* soundManager) : App(manager) {
    this->manager = manager;
    this->soundManager = soundManager;
}

void PetApp::start() {
    Serial.printf("Starting Pet App\n");

    //Setup features
    this->manager->enableVibration();
    this->soundManager->enableAudio();

    //Startup LVGL
    this->manager->watch->lvgl_begin();
    this->manager->watch->openBL();

    //Draw fox :))
    this->img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_auto_size(this->img1, true);
    lv_img_set_src(this->img1, &img_fox_sheet);
    lv_img_set_antialias(this->img1, false);

    lv_obj_set_size(this->img1, 210, 210);
    lv_img_set_offset_x(this->img1, 0);

    lv_obj_align(this->img1, NULL, LV_ALIGN_CENTER, 0, 0);

    //attach touch screen interrupt pin
    pinMode(TP_INT, INPUT);
}

void PetApp::stop() {

}

void PetApp::loop() {

    if (this->isPlaying) {
        //Sound no longer playing
        if (!this->sound->isPlaying()) {
            this->isPlaying = false;
            lv_img_set_offset_x(this->img1, 0);
        }
        //Sound still playing
        else {
            lv_img_set_offset_x(this->img1, 210);
        }

    }

    //Detect screen press
    if (digitalRead(TP_INT) == LOW) {
        
        this->manager->resetSleepAndDimTimer();
        this->chirp();
    }
}

void PetApp::chirp() {

    if (!this->isPlaying) {
        this->manager->vibrate(40);

        this->sound = this->soundManager->playWAVFromMem(sfx_fox_chirp, sizeof(sfx_fox_chirp));

        if (this->sound != nullptr)
            this->isPlaying = true;
    }
}
