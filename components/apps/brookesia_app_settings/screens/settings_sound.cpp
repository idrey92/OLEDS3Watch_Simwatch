#include "../esp_brookesia_app_settings.hpp"

#include "settings.h"

namespace esp_brookesia::apps
{   
    
    void Settings::showSoundScreen()
    {
        clearScreen();

        createHeader("Sound");

        char volumeText[8];
        sprintf(volumeText, "%u%%", settings_get_sound_volume());

        createValueMenuItem(
            screen,
            "Sound",
            settings_get_sound_enabled() ? "On" : "Off",
            soundEnableEventCb);

        createValueMenuItem(
            screen,
            "Volume",
            volumeText,
            volumeEventCb);
    }

    void Settings::soundMenuEventCb(lv_event_t *event)
    {
        auto *self =
            static_cast<Settings *>(lv_event_get_user_data(event));

        self->showSoundScreen();
    }

    void Settings::soundEnableEventCb(lv_event_t *event)
{
    auto *self =
        static_cast<Settings *>(lv_event_get_user_data(event));

    self->clearScreen();

    self->createHeader("Sound");

    self->soundSwitch = lv_switch_create(self->screen);

    lv_obj_center(self->soundSwitch);

    if (settings_get_sound_enabled())
    {
        lv_obj_add_state(self->soundSwitch, LV_STATE_CHECKED);
    }

    self->createSaveButton(
        self->screen,
        saveSoundEventCb);
}

void Settings::saveSoundEventCb(lv_event_t *event)
{
    auto *self =
        static_cast<Settings *>(lv_event_get_user_data(event));

    settings_set_sound_enabled(
        lv_obj_has_state(
            self->soundSwitch,
            LV_STATE_CHECKED));

    self->showSoundScreen();
}

void Settings::volumeEventCb(lv_event_t *event)
{
    auto *self =
        static_cast<Settings *>(lv_event_get_user_data(event));

    self->showVolumeScreen();
}

void Settings::showVolumeScreen()
{
    clearScreen();

    createHeader("Volume");

    // Текущее значение
    char buf[8];
    sprintf(buf, "%u%%", settings_get_sound_volume());

    soundVolumeLabel = lv_label_create(screen);
    lv_label_set_text(soundVolumeLabel, buf);
    lv_obj_set_style_text_font(
        soundVolumeLabel,
        &esp_brookesia_font_maison_neue_book_32,
        0);

    lv_obj_align(
        soundVolumeLabel,
        LV_ALIGN_TOP_MID,
        0,
        110);

    // Слайдер
    soundVolumeSlider = lv_slider_create(screen);

    lv_obj_set_size(soundVolumeSlider, 280, 8);

    lv_slider_set_range(
        soundVolumeSlider,
        0,
        100);

    lv_slider_set_value(
        soundVolumeSlider,
        settings_get_sound_volume(),
        LV_ANIM_OFF);

    lv_obj_align(
        soundVolumeSlider,
        LV_ALIGN_TOP_MID,
        0,
        170);

    lv_obj_add_event_cb(
        soundVolumeSlider,
        volumeSliderEventCb,
        LV_EVENT_VALUE_CHANGED,
        this);

    // Кнопка Save
    lv_obj_t *saveBtn = createSaveButton(
        screen,
        saveVolumeEventCb);

    lv_obj_align(
        saveBtn,
        LV_ALIGN_BOTTOM_MID,
        0,
        -25);
}

void Settings::saveVolumeEventCb(lv_event_t *event)
{
    auto *self =
        static_cast<Settings *>(lv_event_get_user_data(event));

    settings_set_sound_volume(
        lv_slider_get_value(self->soundVolumeSlider));

    self->showSoundScreen();
}

}