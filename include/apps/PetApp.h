#pragma once

#include <lvgl.h>

#include "WatchManager.h"
#include "SoundManager.h"
#include "ui/AnimatedAtlas.h"
#include "App.h"

//Assets
#include "assets/sfx_fox_chirp.h"
LV_IMG_DECLARE(img_fox_sheet);

class PetApp : public App {
    private:

    SoundManager* soundManager = nullptr;
    AnimatedAtlas* foxAtlas;
    PlayingSound* sound = nullptr;

    animationData idleAnimation;
    animationData clickAnimation;

    bool isPlaying = false;
    bool touchDown = false;
    
    public:
    PetApp(WatchManager* manager,SoundManager* soundManager);

    void start();

    void stop();

    void loop();

    void chirp();

};
