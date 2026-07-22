#pragma once

#include "lvgl.h"

#include "../core/sim_core.hpp"
#include <vector>
#include <memory>
#include "stat_widget.hpp"

namespace simwatch
{

class ScreenMain
{
public:

    bool create(
        lv_obj_t *parent,
        SimCore &core);

private:

    lv_obj_t *_screen = nullptr;

    std::vector<std::unique_ptr<StatWidget>> _widgets;
};

}