#include "WatchManager.h"
#include "SoundManager.h"
#include "App.h"
#include "apps/ClockApp.h"

//Assets
LV_FONT_DECLARE(morgnite_bold_64);

ClockApp::ClockApp(WatchManager* manager,SoundManager* soundManager) : App(manager) {
    this->manager = manager;
}

void ClockApp::start() {
    Serial.printf("Starting Clock App\n");

    this->manager->enableVibration();

    //Clear
    lv_obj_clean(lv_scr_act());
    
    this->manager->watch->rtc->check();
    
    lv_obj_set_style_local_bg_color(lv_scr_act(), 
        LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 
        lv_color_hex3(0xf33));
    
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style, LV_STATE_DEFAULT, &morgnite_bold_64);

    this->g_data.hour = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_add_style(this->g_data.hour, LV_OBJ_PART_MAIN, &style);

    lv_label_set_text(this->g_data.hour, "00");
    lv_obj_align(this->g_data.hour, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 10, 30);

    this->g_data.minute = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_add_style(this->g_data.minute, LV_OBJ_PART_MAIN, &style);
    lv_label_set_text(this->g_data.minute, "00");
    lv_obj_align(this->g_data.minute, this->g_data.hour, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    this->g_data.second = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_add_style(this->g_data.second, LV_OBJ_PART_MAIN, &style);
    lv_label_set_text(this->g_data.second, "00");
    lv_obj_align(this->g_data.second, this->g_data.minute, LV_ALIGN_OUT_RIGHT_MID, 9, 0);
    
    setCpuFrequencyMhz(20);
}

void ClockApp::stop() {
}

void ClockApp::loop()
{
    auto curr_datetime = this->manager->watch->rtc->getDateTime();
    lv_label_set_text_fmt(this->g_data.second, "%02u", curr_datetime.second);
    lv_label_set_text_fmt(this->g_data.minute, "%02u", curr_datetime.minute);
    lv_label_set_text_fmt(this->g_data.hour, "%02u", curr_datetime.hour);
}
