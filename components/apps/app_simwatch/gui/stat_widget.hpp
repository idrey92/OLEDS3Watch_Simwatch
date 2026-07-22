#pragma once

#include "lvgl.h"

#include "../core/sim_stat.hpp"

namespace simwatch
{

class StatWidget
{
public:

    bool create(
        lv_obj_t *parent,
        const Stat &stat);

    lv_obj_t *object()
    {
        return _container;
    }

      void update(
        const Stat &stat);

       

private:

    lv_obj_t *_container = nullptr;

    lv_obj_t *_name = nullptr;

    lv_obj_t *_bar_bg = nullptr;

    lv_obj_t *_bar_fill = nullptr;

    lv_obj_t *_value = nullptr;

    lv_obj_t *_header = nullptr;

    lv_obj_t *_title = nullptr; 

    int _width = 220;
};

}