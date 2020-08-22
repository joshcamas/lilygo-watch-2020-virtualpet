#pragma once

#include <lvgl.h>

#include "WatchManager.h"
#include "SoundManager.h"
#include "App.h"

//Assets
#include "assets/sfx_fox_chirp.h"
LV_IMG_DECLARE(img_fox_sheet);

class PetApp : public App {
    private:

    SoundManager* soundManager = nullptr;

    lv_obj_t* img1 = nullptr;
    PlayingSound* sound = nullptr;

    bool isPlaying = false;

    public:
    PetApp(WatchManager* manager,SoundManager* soundManager);

    void start();

    void stop();

    void loop();

    void chirp();

};
