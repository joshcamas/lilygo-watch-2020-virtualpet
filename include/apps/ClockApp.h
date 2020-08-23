#pragma once

#include <lvgl.h>
#include "WatchManager.h"
#include "SoundManager.h"
#include "App.h"

typedef struct {
    lv_obj_t *hour;
    lv_obj_t *minute;
    lv_obj_t *second;
} str_datetime_t;


class ClockApp : public App {
    private:
        str_datetime_t g_data;

    public:

    ClockApp(WatchManager* manager,SoundManager* soundManager);
    
    void start();

    void loop();
    
    void stop();
};
