#pragma once

#include "lvgl.h"
#include "esp_brookesia.hpp"
#include "assets/esp_brookesia_app_settings_assets.h"

namespace esp_brookesia::apps {

    class Settings : public systems::phone::App {
    public:
        Settings();
        ~Settings();

        //SettingsUI ui;
        //SettingsManager manager;

        bool run() override;
        bool back() override;
        bool close() override;
        bool init() override;
        bool deinit() override;

    private:
        lv_obj_t *createSaveButton(
        lv_obj_t *parent,
        lv_event_cb_t cb);

        void refreshLabels();
        void showTimeScreen();
        void showDateScreen();
        static void rollerEventCb(lv_event_t *event);
        

        static void timeEventCb(lv_event_t *event);
        static void dateEventCb(lv_event_t *event);

        static void saveTimeEventCb(lv_event_t *event);
        static void saveDateEventCb(lv_event_t *event);

        static void brightnessEventCb(lv_event_t *event);

        void showBrightnessScreen();

        void clearScreen();
        void showMainScreen();
        void showDateTimeScreen();
        void showDisplayScreen();
        void showSoundScreen();
        void showAboutScreen();
        static void backEventCb(lv_event_t *event);
        void showMainMenu();
        lv_obj_t *createMenuItem(
        lv_obj_t *parent,
        const char *title,
        const char *subtitle,
        lv_event_cb_t cb);
       

        lv_obj_t *hourRoller = nullptr;
        lv_obj_t *minuteRoller = nullptr;
        lv_obj_t *dayRoller = nullptr;
        lv_obj_t *monthRoller = nullptr;
        lv_obj_t *yearRoller = nullptr;
        lv_obj_t *timeoutRoller = nullptr;

        lv_obj_t *alwaysOnSwitch = nullptr;

        lv_obj_t *createHeader(const char *title);

        lv_obj_t *createMenuItem(
            lv_obj_t *parent,
            const char *text,
            lv_event_cb_t cb);

        lv_obj_t *createValueMenuItem(
            lv_obj_t *parent,
            const char *text,
            const char *value,
            lv_event_cb_t cb);

        lv_obj_t *brightnessSlider = nullptr;
        lv_obj_t *brightnessValue = nullptr;
        lv_obj_t* createRow(lv_obj_t* parent, const char* name, lv_obj_t** value_label);
        void showTimeoutScreen();
        void showAlwaysOnScreen();
        void showVolumeScreen();

        
        lv_obj_t *soundSwitch = nullptr;
        
        static void soundMenuEventCb(lv_event_t *event);
        static void soundEnableEventCb(lv_event_t *event);
        static void volumeSliderEventCb(lv_event_t *event);
        static void volumeEventCb(lv_event_t *event);
        static void saveSoundEventCb(lv_event_t *event);
        static void saveVolumeEventCb(lv_event_t *event);
        
        lv_obj_t *soundVolumeSlider = nullptr;

       
        static void alwaysOnEventCb(lv_event_t *);
        static void saveAlwaysOnEventCb(lv_event_t *);
        static void timeoutEventCb(lv_event_t *event);
        static void saveTimeoutEventCb(lv_event_t *event);
        static void stepGoalEventCb(lv_event_t* event);
        static void soundEventCb(lv_event_t* event);
        static void resetEventCb(lv_event_t* event);
        static void dateTimeEventCb(lv_event_t* event);
        static void displayEventCb(lv_event_t* event);
        static void healthEventCb(lv_event_t* event);
        static void aboutEventCb(lv_event_t* event);
        static void brightnessSliderEventCb(lv_event_t *event);
            
        lv_obj_t* screen = nullptr;
        lv_obj_t* brightness_label = nullptr;
        lv_obj_t* timeout_label = nullptr;
        lv_obj_t* sound_label = nullptr;
        lv_obj_t* volume_label = nullptr;
        lv_obj_t* step_goal_label = nullptr;
        lv_obj_t* brightness_slider = nullptr;
        lv_obj_t* timeout_dropdown = nullptr;
        lv_obj_t* sound_switch = nullptr;
        lv_obj_t* volume_slider = nullptr;
        lv_obj_t* step_goal_slider = nullptr;
        lv_obj_t *soundVolumeLabel = nullptr;
    };

}
