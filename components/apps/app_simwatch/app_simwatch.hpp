#pragma once

#include "lvgl.h"
#include "esp_brookesia.hpp"
#include "assets/app_simwatch_assets.h"
#include <memory>
#include "gui/screen_main.hpp"

namespace esp_brookesia::apps {

    class Simwatch : public systems::phone::App {
    public:
        Simwatch();
        ~Simwatch();

        bool run() override;
        bool back() override;
        bool close() override;
        bool init() override;
        bool deinit() override;

        std::unique_ptr<simwatch::ScreenMain> _screen;

    private:
        
    };

}
