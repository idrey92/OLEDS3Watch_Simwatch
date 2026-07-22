#include <iterator>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include "app_watchface.hpp"
#include "rtc_lib.h"
#include "bsp/esp-bsp.h"
#include <math.h>

#define ESP_UTILS_LOG_TAG "App:Watchface"
#include "esp_lib_utils.h"

#define APP_NAME "Clock"

using namespace std;
using namespace esp_brookesia::gui;
using namespace esp_brookesia::systems::phone;

namespace esp_brookesia::apps {

    Watchface::Watchface() :
        App({
        .name = APP_NAME,
        .launcher_icon = gui::StyleImage::IMAGE(&app_watchface_128_128),
        .screen_size = gui::StyleSize::RECT_PERCENT(100, 100),
        .flags = {
            .enable_default_screen = 1,
            .enable_recycle_resource = 0,
            .enable_resize_visual_area = 1,
        },
            },
        {
        .app_launcher_page_index = 0,
        .flags = {
            .enable_navigation_gesture = 1,
        },
        })
    {
    }

    Watchface::~Watchface()
    {
        ESP_UTILS_LOGD("Destroy(@0x%p)", this);
    }


    bool Watchface::run()
    {
        ESP_UTILS_LOGD("Run");


        lv_obj_t* watchface_screen = lv_obj_create(lv_scr_act());
        lv_obj_remove_style_all(watchface_screen);
        lv_obj_set_size(watchface_screen, lv_pct(100), lv_pct(100));

        lv_obj_set_style_bg_color(watchface_screen, lv_color_hex(0x000000), 0);

        lv_obj_t* image = lv_image_create(watchface_screen);
        lv_image_set_src(image, &background_wf);
        lv_obj_set_align(image, LV_ALIGN_CENTER);

        label_hour = lv_label_create(watchface_screen);
        lv_obj_set_y(label_hour, -95);
        lv_obj_set_align(label_hour, LV_ALIGN_CENTER);
        lv_label_set_text(label_hour, "--");
        lv_obj_set_style_text_letter_space(label_hour, 1, 0);
        lv_obj_set_style_text_font(label_hour, &font_numbers_160, 0);
        lv_obj_set_style_text_color(label_hour, lv_color_hex(0xF0B000), LV_PART_MAIN | LV_STATE_DEFAULT);

        label_minute = lv_label_create(watchface_screen);
        lv_obj_set_y(label_minute, 105);
        lv_obj_set_align(label_minute, LV_ALIGN_CENTER);
        lv_label_set_text(label_minute, "--");
        lv_obj_set_style_text_letter_space(label_minute, 1, 0);
        lv_obj_set_style_text_font(label_minute, &font_numbers_160, 0);
        lv_obj_set_style_text_color(label_minute, lv_color_hex(0x90F090), LV_PART_MAIN | LV_STATE_DEFAULT);

        label_second = lv_label_create(watchface_screen);
        lv_obj_set_align(label_second, LV_ALIGN_CENTER);
        lv_label_set_text(label_second, "--");
        lv_obj_set_style_text_letter_space(label_second, 1, 0);
        lv_obj_set_style_text_font(label_second, &font_numbers_80, 0);
        lv_obj_set_style_text_color(label_second, lv_color_hex(0x909090), LV_PART_MAIN | LV_STATE_DEFAULT);

        //
        battery_icon = lv_label_create(watchface_screen);

        lv_label_set_text(
            battery_icon,
            LV_SYMBOL_BATTERY_FULL);

        lv_obj_set_style_text_font(
            battery_icon,
            &lv_font_montserrat_20,
            0);

        lv_obj_set_style_text_color(
            battery_icon,
            lv_color_hex(0x55ff55),
            0);

        lv_obj_align(
            battery_icon,
            LV_ALIGN_BOTTOM_MID,
            -28,
            -40);
        
        
        battery_percent = lv_label_create(watchface_screen);

        lv_obj_set_style_text_font(
            battery_percent,
            &lv_font_montserrat_20,
            0);

        lv_obj_align(
            battery_percent,
            LV_ALIGN_BOTTOM_MID,
            22,
            -40);

        
        //

        lv_obj_t* date_cont = lv_obj_create(watchface_screen);
        lv_obj_remove_style_all(date_cont);
        lv_obj_set_size(date_cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_x(date_cont, -20);
        lv_obj_set_align(date_cont, LV_ALIGN_RIGHT_MID);
        lv_obj_set_flex_flow(date_cont, LV_FLEX_FLOW_COLUMN);


        label_date = lv_label_create(date_cont);
        lv_label_set_text(label_date, "--/--");
        lv_obj_set_style_text_letter_space(label_date, 1, 0);
        lv_obj_set_style_text_font(label_date, &font_normal_32, 0);
        lv_obj_set_style_text_color(label_date, lv_color_hex(0xc0c0c0), LV_PART_MAIN | LV_STATE_DEFAULT);

        label_weekday = lv_label_create(date_cont);
        lv_label_set_text(label_weekday, "---");
        lv_obj_set_style_text_letter_space(label_weekday, 3, 0);
        lv_obj_set_style_text_font(label_weekday, &font_bold_32, 0);
        lv_obj_set_style_text_color(label_weekday, lv_color_hex(0xc0c0c0), LV_PART_MAIN | LV_STATE_DEFAULT);

        updateTime();
        time_timer = lv_timer_create(timeTimerCb, 1000, this);
        charge_timer = lv_timer_create(
            chargeTimerCb,
            120,
            this);

        return true;
    }

    bool Watchface::back()
    {
        ESP_UTILS_LOGD("Back");
        ESP_UTILS_CHECK_FALSE_RETURN(notifyCoreClosed(), false, "Notify core closed failed");
        return true;
    }

    bool Watchface::close()
    {
        ESP_UTILS_LOGD("Close");
        if (time_timer != nullptr) {
            lv_timer_del(time_timer);
            time_timer = nullptr;
        }
        if (charge_timer)
        {
            lv_timer_del(charge_timer);
            charge_timer = nullptr;
        }

        return true;
    }

    bool Watchface::init()
    {
        ESP_UTILS_LOGD("Init");

        return true;
    }

    bool Watchface::deinit()
    {
        ESP_UTILS_LOGD("Deinit");
        if (time_timer != nullptr) {
            lv_timer_del(time_timer);
            time_timer = nullptr;
        }
        if (charge_timer)
        {
            lv_timer_del(charge_timer);
            charge_timer = nullptr;
        }
        return true;

    }

    void Watchface::updateTime()
    {
        struct tm now = {};
        rtc_get_time(&now);

        char buf[16];
        snprintf(buf, sizeof(buf), "%02d", now.tm_hour);
        lv_label_set_text(label_hour, buf);
        snprintf(buf, sizeof(buf), "%02d", now.tm_min);
        lv_label_set_text(label_minute, buf);
        snprintf(buf, sizeof(buf), "%02d", now.tm_sec);
        lv_label_set_text(label_second, buf);
        snprintf(buf, sizeof(buf), "%02d/%02d", now.tm_mday, now.tm_mon + 1);
        lv_label_set_text(label_date, buf);
        lv_label_set_text(label_weekday, rtc_get_weekday_short_string());
    }

    void Watchface::timeTimerCb(lv_timer_t* timer)
    {
        auto* self = static_cast<Watchface*>(lv_timer_get_user_data(timer));
        if (self != nullptr) {
            self->updateTime();
        }
    }

    void Watchface::chargeTimerCb(lv_timer_t *timer)
{
    auto *self = static_cast<Watchface *>(lv_timer_get_user_data(timer));

    uint8_t percent = bsp_power_get_battery_percent();

    char buf[16];
    snprintf(buf, sizeof(buf), "%u%%", percent);
    lv_label_set_text(self->battery_percent, buf);

    if (bsp_power_is_charging())
    {
        // Во время зарядки всегда показываем молнию
        lv_label_set_text(self->battery_icon, LV_SYMBOL_CHARGE);
        return;
    }

    // Выбор значка батареи по уровню заряда
    if (percent >= 90)
    {
        lv_label_set_text(self->battery_icon, LV_SYMBOL_BATTERY_FULL);
    }
    else if (percent >= 65)
    {
        lv_label_set_text(self->battery_icon, LV_SYMBOL_BATTERY_3);
    }
    else if (percent >= 40)
    {
        lv_label_set_text(self->battery_icon, LV_SYMBOL_BATTERY_2);
    }
    else if (percent >= 15)
    {
        lv_label_set_text(self->battery_icon, LV_SYMBOL_BATTERY_1);
    }
    else
    {
        lv_label_set_text(self->battery_icon, LV_SYMBOL_BATTERY_EMPTY);
    }
}

    ESP_UTILS_REGISTER_PLUGIN(systems::base::App, Watchface, APP_NAME)

}
