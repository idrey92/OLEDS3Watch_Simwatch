#include "../esp_brookesia_app_settings.hpp"

// #include "../ui/settings_ui.hpp"

#include "settings.h"
//#include "display_manager.h"
#include "rtc_lib.h"

using namespace esp_brookesia::gui;
using namespace esp_brookesia::systems::phone;
using namespace esp_brookesia::systems::base;

namespace esp_brookesia::apps
{
   void Settings::showDisplayScreen()
    {
        clearScreen();

        createHeader("Display");

        char timeout[16];

        switch (settings_get_display_timeout())
        {
            case SETTINGS_DISPLAY_TIMEOUT_10S:
                strcpy(timeout, "10 sec");
                break;

            case SETTINGS_DISPLAY_TIMEOUT_20S:
                strcpy(timeout, "20 sec");
                break;

            case SETTINGS_DISPLAY_TIMEOUT_30S:
                strcpy(timeout, "30 sec");
                break;

            case SETTINGS_DISPLAY_TIMEOUT_1MIN:
                strcpy(timeout, "1 min");
                break;

            default:
                strcpy(timeout, "30 sec");
                break;
        }

        createValueMenuItem(
            screen,
            "Brightness",
            "",
            brightnessEventCb);

        createValueMenuItem(
            screen,
            "Screen Timeout",
            timeout,
            timeoutEventCb);

        createValueMenuItem(
            screen,
            "Always On",
            settings_get_always_on() ? "On" : "Off",
            alwaysOnEventCb);
    }

    void Settings::showBrightnessScreen()
    {
        clearScreen();

        lv_obj_t *back = lv_label_create(screen);
        lv_label_set_text(back, LV_SYMBOL_LEFT " Back");
        lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(back, backEventCb, LV_EVENT_CLICKED, this);

        lv_obj_t *title = lv_label_create(screen);
        lv_label_set_text(title, "Brightness");

        lv_obj_set_style_text_font(
            title,
            &esp_brookesia_font_maison_neue_book_32,
            0);

        brightnessSlider = lv_slider_create(screen);

        lv_obj_set_width(brightnessSlider, 300);

        lv_slider_set_range(
            brightnessSlider,
            10,
            100);

        lv_slider_set_value(
            brightnessSlider,
            settings_get_brightness(),
            LV_ANIM_OFF);

        lv_obj_add_event_cb(
            brightnessSlider,
            brightnessSliderEventCb,
            LV_EVENT_VALUE_CHANGED,
            this);  
            
            brightnessValue = lv_label_create(screen);

            char txt[8];

            sprintf(
                txt,
                "%d%%",
                settings_get_brightness());

            lv_label_set_text(
                brightnessValue,
                txt);
    }

     void Settings::showTimeoutScreen()
    {
        clearScreen();

        //==========================================================
        // Header
        //==========================================================

        createHeader("Screen Timeout");

        //==========================================================
        // Roller
        //==========================================================

        lv_obj_t *rollerBox = lv_obj_create(screen);

        lv_obj_remove_style_all(rollerBox);

        lv_obj_set_width(rollerBox, lv_pct(100));
        lv_obj_set_height(rollerBox, 270);

        lv_obj_set_layout(rollerBox, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(rollerBox, LV_FLEX_FLOW_COLUMN);

        lv_obj_set_flex_align(
            rollerBox,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER);

        static const char *options =
            "10 seconds\n"
            "20 seconds\n"
            "30 seconds\n"
            "1 minute";

        timeoutRoller = lv_roller_create(rollerBox);

        lv_obj_set_size(timeoutRoller, 220, 220);

        lv_roller_set_visible_row_count(timeoutRoller, 4);

        lv_roller_set_options(
            timeoutRoller,
            options,
            LV_ROLLER_MODE_NORMAL);

        int index = 2;

        switch (settings_get_display_timeout())
        {
            case SETTINGS_DISPLAY_TIMEOUT_10S:
                index = 0;
                break;

            case SETTINGS_DISPLAY_TIMEOUT_20S:
                index = 1;
                break;

            case SETTINGS_DISPLAY_TIMEOUT_30S:
                index = 2;
                break;

            case SETTINGS_DISPLAY_TIMEOUT_1MIN:
                index = 3;
                break;
        }

        lv_roller_set_selected(
            timeoutRoller,
            index,
            LV_ANIM_OFF);

        lv_obj_add_event_cb(
            timeoutRoller,
            rollerEventCb,
            LV_EVENT_ALL,
            this);

        //==========================================================
        // Save
        //==========================================================

        createSaveButton(
            screen,
            saveTimeoutEventCb);
    }

    void Settings::brightnessEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));
        self->showBrightnessScreen();
    }

    void Settings::timeoutEventCb(lv_event_t *event)
    {
        auto *self =
            static_cast<Settings *>(lv_event_get_user_data(event));

        self->showTimeoutScreen();
    }

    void Settings::brightnessSliderEventCb(lv_event_t *event)
    {
        auto *self =
            static_cast<Settings *>(lv_event_get_user_data(event));

        int value = lv_slider_get_value(self->brightnessSlider);

        settings_set_brightness(value);

        char txt[8];
        sprintf(txt, "%d%%", value);

        lv_label_set_text(self->brightnessValue, txt);
    }

    void Settings::saveTimeoutEventCb(lv_event_t *event)
    {
        auto *self =
            static_cast<Settings *>(lv_event_get_user_data(event));

        switch (lv_roller_get_selected(self->timeoutRoller))
        {
            case 0:
                settings_set_display_timeout(
                    SETTINGS_DISPLAY_TIMEOUT_10S);
                break;

            case 1:
                settings_set_display_timeout(
                    SETTINGS_DISPLAY_TIMEOUT_20S);
                break;

            case 2:
                settings_set_display_timeout(
                    SETTINGS_DISPLAY_TIMEOUT_30S);
                break;

            case 3:
                settings_set_display_timeout(
                    SETTINGS_DISPLAY_TIMEOUT_1MIN);
                break;
        }

        self->showDisplayScreen();
    }

    void Settings::showAlwaysOnScreen()
    {
    clearScreen();

    createHeader("Always On");

    lv_obj_t *cont = lv_obj_create(screen);

    lv_obj_remove_style_all(cont);

    lv_obj_set_size(cont, lv_pct(100), 250);

    lv_obj_center(cont);

    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_flex_align(
        cont,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER);

    lv_obj_t *label = lv_label_create(cont);
    lv_label_set_text(label, "Keep display always on");

    alwaysOnSwitch = lv_switch_create(cont);

    lv_obj_add_state(
        alwaysOnSwitch,
        settings_get_always_on() ?
            LV_STATE_CHECKED :
            0);

    createSaveButton(
        screen,
        saveAlwaysOnEventCb);
    }

    void Settings::alwaysOnEventCb(lv_event_t *event)
    {
    auto *self =
        static_cast<Settings *>(lv_event_get_user_data(event));

    self->showAlwaysOnScreen();
    }

    void Settings::saveAlwaysOnEventCb(lv_event_t *event)
    {
        auto *self =
            static_cast<Settings *>(lv_event_get_user_data(event));

        settings_set_always_on(
            lv_obj_has_state(
                self->alwaysOnSwitch,
                LV_STATE_CHECKED));

        self->showDisplayScreen();
    }


    }