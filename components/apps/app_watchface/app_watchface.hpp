#pragma once

#include "lvgl.h"
#include "esp_brookesia.hpp"
#include "assets/app_watchface_assets.h"

namespace esp_brookesia::apps {

    class Watchface : public systems::phone::App {
    public:
        Watchface();
        ~Watchface();

        bool run() override;
        bool back() override;
        bool close() override;
        bool init() override;
        bool deinit() override;

    private:
        void updateTime();
        static void timeTimerCb(lv_timer_t* timer);

        lv_obj_t* label_hour = nullptr;
        lv_obj_t* label_minute = nullptr;
        lv_obj_t* label_second = nullptr;
        lv_obj_t* label_date = nullptr;
        lv_obj_t* label_weekday = nullptr;
        lv_timer_t* time_timer = nullptr;

        lv_obj_t *battery_label = nullptr;
        lv_timer_t *charge_timer = nullptr;

        lv_obj_t *battery_icon = nullptr;
        lv_obj_t *battery_percent = nullptr;
        
       
        static void chargeTimerCb(lv_timer_t *timer);
    };

}
