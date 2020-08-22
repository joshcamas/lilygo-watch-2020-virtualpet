/*#ifndef CLOCKAPP_H
#define CLOCKAPP_H

#include <lvgl.h>

#include "WatchManager.h"
#include "SoundManager.h"
#include "App.h"

//Assets
LV_FONT_DECLARE(morgnite_bold_64);

typedef struct {
    lv_obj_t *hour;
    lv_obj_t *minute;
    lv_obj_t *second;
} str_datetime_t;


class ClockApp : public App {
    private:

    public:
    
    static str_datetime_t g_data;

    void start() {
        Serial.printf("Starting Clock App\n");

        //Startup LVGL
        this->manager->watch->lvgl_begin();
        this->manager->watch->openBL();
        
        this->manager->watch->rtc->check();

        static lv_style_t style;
        lv_style_init(&style);
        lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_style_set_text_font(&style, LV_STATE_DEFAULT, &morgnite_bold_64);

        g_data.hour = lv_label_create(lv_scr_act(), nullptr);
        lv_obj_add_style(g_data.hour, LV_OBJ_PART_MAIN, &style);

        lv_label_set_text(g_data.hour, "00");
        lv_obj_align(g_data.hour, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 10, 30);

        g_data.minute = lv_label_create(lv_scr_act(), nullptr);
        lv_obj_add_style(g_data.minute, LV_OBJ_PART_MAIN, &style);
        lv_label_set_text(g_data.minute, "00");
        lv_obj_align(g_data.minute, g_data.hour, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

        g_data.second = lv_label_create(lv_scr_act(), nullptr);
        lv_obj_add_style(g_data.second, LV_OBJ_PART_MAIN, &style);
        lv_label_set_text(g_data.second, "00");
        lv_obj_align(g_data.second, g_data.minute, LV_ALIGN_OUT_RIGHT_MID, 9, 0);
        
        //setCpuFrequencyMhz(20);
    }

    void loop()
    {
        auto curr_datetime = this->manager->watch->rtc->getDateTime();
        lv_label_set_text_fmt(g_data.second, "%02u", curr_datetime.second);
        lv_label_set_text_fmt(g_data.minute, "%02u", curr_datetime.minute);
        lv_label_set_text_fmt(g_data.hour, "%02u", curr_datetime.hour);
    }
};

#endif*/