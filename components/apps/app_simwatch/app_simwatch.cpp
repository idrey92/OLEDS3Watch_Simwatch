#include <iterator>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include "app_simwatch.hpp"
#include "rtc_lib.h"
#include "bsp/esp-bsp.h"
#include <math.h>
#include "core/sim_core.hpp"
#include "localization/sim_localization.hpp"
#include "gui/screen_main.hpp"


#define ESP_UTILS_LOG_TAG "App:Simwatch"
#include "esp_lib_utils.h"

#define APP_NAME "Simwatch"

using namespace std;
using namespace esp_brookesia::gui;
using namespace esp_brookesia::systems::phone;

static simwatch::SimCore core;

namespace esp_brookesia::apps {



    Simwatch::Simwatch() :
        App({
        .name = APP_NAME,
        .launcher_icon = gui::StyleImage::IMAGE(&app_simwatch_128_128),
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

    Simwatch::~Simwatch()
    {
        ESP_UTILS_LOGD("Destroy(@0x%p)", this);
    }


    bool Simwatch::run()
    {
        ESP_UTILS_LOGD("Run");

        core.begin();

        _screen = std::make_unique<simwatch::ScreenMain>();
        ESP_LOGI("SIMWATCH", "Before create");

        _screen->create(
            lv_scr_act(),
            core);

        ESP_LOGI("SIMWATCH", "After create");
        return true;
    }

    bool Simwatch::back()
    {
        ESP_UTILS_LOGD("Back");
        ESP_UTILS_CHECK_FALSE_RETURN(notifyCoreClosed(), false, "Notify core closed failed");
        return true;
    }

    bool Simwatch::close()
    {
        ESP_UTILS_LOGD("Close");
        _screen.reset();
        return true;
    }

    bool Simwatch::init()
    {
        ESP_UTILS_LOGD("Init");

        return true;
    }

    bool Simwatch::deinit()
    {
        ESP_UTILS_LOGD("Deinit");
                
        return true;

    }

    ESP_UTILS_REGISTER_PLUGIN(systems::base::App, Simwatch, APP_NAME)

}
