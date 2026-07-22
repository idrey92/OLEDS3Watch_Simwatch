#include <iterator>
#include <thread>
#include <unistd.h>
#include <stdio.h>
//#include "esp_brookesia_app_settings_utils.hpp"
#include "esp_brookesia_app_settings.hpp"
#include "settings.h"
#include "rtc_lib.h"

#define ESP_UTILS_LOG_TAG "BS:App:Settings"
#include "esp_lib_utils.h"

#define APP_NAME "Settings"

using namespace std;
using namespace esp_brookesia::gui;
using namespace esp_brookesia::systems::phone;
using namespace esp_brookesia::systems::base;

namespace esp_brookesia::apps {

    Settings::Settings() :
        App({
        .name = APP_NAME,
        .launcher_icon = gui::StyleImage::IMAGE(&esp_brookesia_app_icon_launcher_settings_112_112),
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
    {}

    Settings::~Settings()
    {
        ESP_UTILS_LOGD("Destroy(@0x%p)", this);
    }


    bool Settings::run()
    {
        ESP_UTILS_LOGD("Run");

        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x070A0D), 0);

        screen = lv_obj_create(lv_scr_act());
        lv_obj_remove_style_all(screen);
        lv_obj_set_size(screen, lv_pct(100), lv_pct(100));
        lv_obj_set_style_bg_color(screen, lv_color_hex(0x070A0D), 0);
        lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
        lv_obj_set_style_pad_left(screen, 22, 0);
        lv_obj_set_style_pad_right(screen, 22, 0);
        lv_obj_set_style_pad_top(screen, 18, 0);
        lv_obj_set_style_pad_bottom(screen, 24, 0);
        lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_row(screen, 12, 0);
        
        showMainScreen();
       
        return true;
    }

    bool Settings::back()
    {
        ESP_UTILS_LOGD("Back");
        ESP_UTILS_CHECK_FALSE_RETURN(notifyCoreClosed(), false, "Notify core closed failed");
        return true;
    }

    bool Settings::close()
    {
        ESP_UTILS_LOGD("Close");
        screen = nullptr;

        return true;
    }

    bool Settings::init()
    {
        ESP_UTILS_LOGD("Init");

        return true;
    }

    bool Settings::deinit()
    {
        ESP_UTILS_LOGD("Deinit");

        return true;
    }


    void Settings::dateTimeEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));
        self->showDateTimeScreen();
    }


    void Settings::displayEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));
        self->showDisplayScreen();
    }

    
    void Settings::healthEventCb(lv_event_t* event)
    {
        LV_UNUSED(event);
        printf("Health\n");
    }

    void Settings::aboutEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));
        self->showAboutScreen();
    }

    

    lv_obj_t* Settings::createRow(lv_obj_t* parent, const char* name, lv_obj_t** value_label)
    {
        lv_obj_t* row = lv_obj_create(parent);
        lv_obj_remove_style_all(row);
        lv_obj_set_size(row, lv_pct(100), 56);
        lv_obj_set_style_pad_column(row, 8, 0);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t* labels = lv_obj_create(row);
        lv_obj_remove_style_all(labels);
        lv_obj_set_size(labels, 130, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(labels, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_row(labels, 2, 0);

        lv_obj_t* name_label = lv_label_create(labels);
        lv_label_set_text(name_label, name);
        lv_obj_set_style_text_color(name_label, lv_color_hex(0xF3F7FA), 0);
        lv_obj_set_style_text_font(name_label, &lv_font_montserrat_18, 0);

        *value_label = lv_label_create(labels);
        lv_obj_set_style_text_color(*value_label, lv_color_hex(0x9BA8B5), 0);
        lv_obj_set_style_text_font(*value_label, &lv_font_montserrat_14, 0);

        return row;
    }

    lv_obj_t *Settings::createMenuItem(
        lv_obj_t *parent,
        const char *title,
        const char *subtitle,
        lv_event_cb_t cb)
    {
    lv_obj_t *btn = lv_btn_create(parent);

    lv_obj_set_width(btn, lv_pct(100));
    lv_obj_set_height(btn, 68);

    lv_obj_set_style_radius(btn, 12, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x141A20), 0);
    lv_obj_set_style_border_width(btn, 0, 0);

    //lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, this);
    if (cb != nullptr)
    {
        lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, this);
    }
    lv_obj_set_layout(btn, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW);

    lv_obj_set_style_pad_left(btn, 16, 0);
    lv_obj_set_style_pad_right(btn, 16, 0);

    lv_obj_set_flex_align(
        btn,
        LV_FLEX_ALIGN_SPACE_BETWEEN,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER);

    //------------------------------------------------

    lv_obj_t *left = lv_obj_create(btn);

    lv_obj_remove_style_all(left);

    lv_obj_set_flex_grow(left, 1);

    lv_obj_set_layout(left, LV_LAYOUT_FLEX);

    lv_obj_set_size(left, LV_PCT(100), LV_SIZE_CONTENT);

    lv_obj_set_flex_flow(left, LV_FLEX_FLOW_COLUMN);

    lv_obj_t *name = lv_label_create(left);

    lv_label_set_text(name, title);

    lv_obj_set_style_text_font(
        name,
        &lv_font_montserrat_20,
        0);

    lv_obj_set_style_text_color(
        name,
        lv_color_white(),
        0);

    lv_obj_t *sub = lv_label_create(left);

    lv_label_set_text(sub, subtitle);

    lv_obj_set_style_text_color(
        sub,
        lv_palette_lighten(LV_PALETTE_GREY,2),
        0);

    //---------------------------------------------

    lv_obj_t *arrow = lv_label_create(btn);

    lv_label_set_text(arrow, LV_SYMBOL_RIGHT);

    lv_obj_set_style_text_font(
        arrow,
        &lv_font_montserrat_20,
        0);

    return btn;
    }

    
    lv_obj_t *Settings::createValueMenuItem(
        lv_obj_t *parent,
        const char *title,
        const char *value,
        lv_event_cb_t cb)
    {
        lv_obj_t *btn = lv_btn_create(parent);
        lv_obj_set_style_bg_color(
    btn,
    lv_color_hex(0x2C2C2E),
    LV_STATE_PRESSED);
        lv_obj_set_width(btn, lv_pct(100));
        lv_obj_set_height(btn, 68);

        lv_obj_set_style_radius(btn, 12, 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x141A20), 0);
        lv_obj_set_style_border_width(btn, 0, 0);
        

        if (cb != nullptr)
        {
            lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, this);
        }

        lv_obj_set_layout(btn, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW);

        lv_obj_set_style_pad_left(btn, 16, 0);
        lv_obj_set_style_pad_right(btn, 16, 0);

        lv_obj_set_flex_align(
            btn,
            LV_FLEX_ALIGN_SPACE_BETWEEN,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER);

        // Левая часть
        lv_obj_t *name = lv_label_create(btn);
        lv_label_set_text(name, title);
        lv_obj_set_style_text_font(name, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(name, lv_color_white(), 0);

        // Правая часть
        lv_obj_t *right = lv_obj_create(btn);
        lv_obj_remove_style_all(right);

        lv_obj_set_layout(right, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(right, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_pad_column(right, 8, 0);

        lv_obj_t *valueLabel = lv_label_create(right);
        lv_label_set_text(valueLabel, value);
        lv_obj_set_style_text_font(valueLabel, &lv_font_montserrat_18, 0);
        lv_obj_set_style_text_color(valueLabel,
                                    lv_palette_lighten(LV_PALETTE_GREY, 2),
                                    0);

        lv_obj_t *arrow = lv_label_create(right);
        lv_label_set_text(arrow, LV_SYMBOL_RIGHT);
        lv_obj_set_style_text_font(
    arrow,
    &lv_font_montserrat_18,
    0);
        lv_obj_set_style_text_color(
        arrow,
        lv_color_hex(0x8E8E93),
        0);
        return btn;
    }

    void Settings::refreshLabels()
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%u%%", settings_get_brightness());
        lv_label_set_text(brightness_label, buf);

        uint32_t timeout = settings_get_display_timeout();
        snprintf(buf, sizeof(buf), timeout == SETTINGS_DISPLAY_TIMEOUT_1MIN ? "1 min" : "%u sec", (unsigned)(timeout / 1000));
        lv_label_set_text(timeout_label, buf);

        lv_label_set_text(sound_label, settings_get_sound() ? "On" : "Off");

        snprintf(buf, sizeof(buf), "%u%%", settings_get_notify_volume());
        lv_label_set_text(volume_label, buf);

        snprintf(buf, sizeof(buf), "%lu steps", (unsigned long)settings_get_step_goal());
        lv_label_set_text(step_goal_label, buf);
    }
   
    void Settings::volumeSliderEventCb(lv_event_t *event)
{
    auto *self =
        static_cast<Settings *>(lv_event_get_user_data(event));

    uint8_t value =
        lv_slider_get_value(self->soundVolumeSlider);

    char buf[8];
    sprintf(buf, "%u%%", value);

    lv_label_set_text(
        self->soundVolumeLabel,
        buf);
}

    void Settings::stepGoalEventCb(lv_event_t* event)
    {
        auto* self = static_cast<Settings*>(lv_event_get_user_data(event));
        uint32_t value = (uint32_t)lv_slider_get_value((lv_obj_t*)lv_event_get_target(event));
        settings_set_step_goal((value / 500) * 500);
        lv_slider_set_value(self->step_goal_slider, settings_get_step_goal(), LV_ANIM_OFF);
        self->refreshLabels();
    }
    
    void Settings::soundEventCb(lv_event_t* event)
    {
        auto* self = static_cast<Settings*>(lv_event_get_user_data(event));
        settings_set_sound(lv_obj_has_state((lv_obj_t*)lv_event_get_target(event), LV_STATE_CHECKED));
        self->refreshLabels();
    }

    void Settings::resetEventCb(lv_event_t* event)
    {
        auto* self = static_cast<Settings*>(lv_event_get_user_data(event));
        settings_reset_defaults();
        lv_slider_set_value(self->brightness_slider, settings_get_brightness(), LV_ANIM_OFF);
        lv_dropdown_set_selected(self->timeout_dropdown, 2);
        if (settings_get_sound()) {
            lv_obj_add_state(self->sound_switch, LV_STATE_CHECKED);
        } else {
            lv_obj_remove_state(self->sound_switch, LV_STATE_CHECKED);
        }
        lv_slider_set_value(self->volume_slider, settings_get_notify_volume(), LV_ANIM_OFF);
        lv_slider_set_value(self->step_goal_slider, settings_get_step_goal(), LV_ANIM_OFF);
        self->refreshLabels();
    }

    void Settings::clearScreen()
    {
        while (lv_obj_get_child_count(screen) > 0)
        {
            lv_obj_del(lv_obj_get_child(screen, 0));
        }
    }

    void Settings::showMainScreen()
    {
        clearScreen();

        lv_obj_t* title = lv_label_create(screen);
        lv_label_set_text(title, "Settings");
        lv_obj_set_style_text_font(title, &esp_brookesia_font_maison_neue_book_32, 0);
        lv_obj_set_style_text_color(title, lv_color_hex(0xF3F7FA), 0);

        createMenuItem(
        screen,
        "Date & Time",
        "Set date and time",
        dateTimeEventCb);
        
        createMenuItem(
        screen,
        "Display",
        "Brightness, timeout",
        displayEventCb);

        createMenuItem(
        screen,
        "Sound",
        "Volume, notifications",
        soundMenuEventCb);

        createMenuItem(
        screen,
        "Health",
        "Step goal",
        healthEventCb);

        createMenuItem(
        screen,
        "About",
        "Watch information",
        aboutEventCb);
    }

    
   
    void Settings::showAboutScreen()
    {
        printf("About\n");
    }

    void Settings::backEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));
        self->showMainScreen();
    }
      
    void Settings::rollerEventCb(lv_event_t *event)
        {
            auto *self = static_cast<Settings *>(lv_event_get_user_data(event));

            if (self == nullptr)
                return;

            switch (lv_event_get_code(event))
            {
                case LV_EVENT_PRESSED:
                {
                    printf("Roller pressed\n");

                    lv_obj_clear_flag(
                        self->screen,
                        LV_OBJ_FLAG_GESTURE_BUBBLE);

                    break;
                }

                case LV_EVENT_RELEASED:
                case LV_EVENT_PRESS_LOST:
                {
                    printf("Roller released\n");

                    lv_obj_add_flag(
                        self->screen,
                        LV_OBJ_FLAG_GESTURE_BUBBLE);

                    break;
                }

                default:
                    break;
            }
        }

    lv_obj_t *Settings::createHeader(const char *titleText)
    {
        lv_obj_t *back = lv_label_create(screen);
        lv_label_set_text(back, LV_SYMBOL_LEFT " Back");

        lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(back, backEventCb, LV_EVENT_CLICKED, this);

        lv_obj_t *title = lv_label_create(screen);

        lv_label_set_text(title, titleText);

        lv_obj_set_style_text_font(
            title,
            &esp_brookesia_font_maison_neue_book_32,
            0);

        return title;
    }

    lv_obj_t *Settings::createSaveButton(
    lv_obj_t *parent,
    lv_event_cb_t cb)
    {
        lv_obj_t *btn = lv_btn_create(parent);

        lv_obj_set_size(btn, 180, 52);

        lv_obj_add_event_cb(
            btn,
            cb,
            LV_EVENT_CLICKED,
            this);

        lv_obj_t *label = lv_label_create(btn);

        lv_label_set_text(label, "Save");

        lv_obj_center(label);

        return btn;
    }

    ESP_UTILS_REGISTER_PLUGIN(systems::base::App, Settings, APP_NAME)
}
