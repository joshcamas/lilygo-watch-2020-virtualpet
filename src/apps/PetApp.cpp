
#include <lvgl.h>

#include "apps/PetApp.h"
#include "WatchManager.h"
#include "SoundManager.h"
#include "ui/AnimatedAtlas.h"
#include "App.h"

//Assets
#include "assets/sfx_fox_chirp.h"

LV_IMG_DECLARE(img_fox_sheet);

PetApp::PetApp(WatchManager *manager, SoundManager *soundManager) : App(manager)
{
    this->manager = manager;
    this->soundManager = soundManager;
}

void PetApp::start()
{
    Serial.printf("Starting Pet App\n");

    //Setup features
    this->soundManager->enableAudio();

    //Clear
    lv_obj_clean(lv_scr_act());

    //Background color
    lv_obj_set_style_local_bg_color(lv_scr_act(),
                                    LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_WHITE);

    //Create image
    lv_obj_t *foxImage = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(foxImage, &img_fox_sheet);
    lv_img_set_antialias(foxImage, false);

    //Idle Animation
    this->idleAnimation.frameSpeed = 1000;
    this->idleAnimation.frames = new int[2]{0, 1};
    this->idleAnimation.frameCount = 2;

    //Click Animation
    this->clickAnimation.frames = new int[1]{2};
    this->clickAnimation.frameCount = 1;
    this->clickAnimation.frameSpeed = 200;

    //Atlas
    this->foxAtlas = new AnimatedAtlas(foxImage, 210, 210);
    this->foxAtlas->PlayAnimation(this->idleAnimation);

    lv_obj_align(foxImage, NULL, LV_ALIGN_CENTER, 0, 0);

    setCpuFrequencyMhz(160);
}

void PetApp::stop()
{
}

void PetApp::loop()
{

    if (this->isPlaying)
    {
        //Sound no longer playing
        if (!this->sound->isPlaying())
        {
            this->isPlaying = false;
        }
    }

    int16_t x, y;
    if (this->manager->allowTouchInput() && this->manager->watch->getTouch(x, y))
    {

        this->manager->resetSleepAndDimTimer();
        this->chirp();

        Serial.print(x);
        Serial.print(",");
        Serial.print(y);
        Serial.print("\n");
    }

    this->foxAtlas->update();
}

void PetApp::chirp()
{

    if (!this->isPlaying)
    {
        this->manager->vibrate(40);

        this->foxAtlas->PlayOverlayAnimation(this->clickAnimation);

        this->sound = this->soundManager->playWAVFromMem(sfx_fox_chirp, sizeof(sfx_fox_chirp));

        if (this->sound != nullptr)
            this->isPlaying = true;
    }
}
