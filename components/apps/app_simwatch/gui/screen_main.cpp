#include "screen_main.hpp"

#include "stat_widget.hpp"

using namespace simwatch;

bool ScreenMain::create(
    lv_obj_t *parent,
    SimCore &core)
{
    _screen = lv_obj_create(parent);

    lv_obj_remove_style_all(_screen);

    lv_obj_set_style_pad_all(_screen, 16, 0);
    lv_obj_set_style_pad_row(_screen, 12, 0);
    lv_obj_set_scrollbar_mode(_screen, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_size(
        _screen,
        lv_pct(100),
        lv_pct(100));

    lv_obj_set_flex_flow(
        _screen,
        LV_FLEX_FLOW_COLUMN);
  

    for (auto &stat : core.stats())
    {
        auto widget = std::make_unique<StatWidget>();

        widget->create(
            _screen,
            stat);

        _widgets.push_back(
            std::move(widget));
    }

    return true;
}