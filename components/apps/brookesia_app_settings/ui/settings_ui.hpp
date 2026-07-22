#pragma once

#include "../esp_brookesia_app_settings.hpp"

namespace esp_brookesia::apps
{
    lv_obj_t *createHeader(
        Settings *self,
        lv_obj_t *parent,
        const char *title);

    lv_obj_t *createBackButton(
        Settings *self,
        lv_obj_t *parent);

    lv_obj_t *createSaveButton(
        Settings *self,
        lv_obj_t *parent,
        lv_event_cb_t cb);
}