#include "stat_widget.hpp"

#include "../localization/sim_localization.hpp"

using namespace simwatch;

bool StatWidget::create(
    lv_obj_t *parent,
    const Stat &stat)
{
    _container = lv_obj_create(parent);

    _header = lv_obj_create(_container);

lv_obj_remove_style_all(_header);

lv_obj_set_width(_header, lv_pct(100));

lv_obj_set_height(_header, LV_SIZE_CONTENT);

lv_obj_set_flex_flow(
    _header,
    LV_FLEX_FLOW_ROW);

lv_obj_set_flex_align(
    _header,
    LV_FLEX_ALIGN_SPACE_BETWEEN,
    LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_pad_all(_container, 12, 0);
    lv_obj_set_style_pad_left(_container, 16, 0);
    lv_obj_set_style_pad_right(_container, 16, 0);
    lv_obj_set_style_pad_top(_container, 10, 0);
    lv_obj_set_style_pad_bottom(_container, 10, 0);
    lv_obj_set_style_pad_row(_container, 8, 0);

    lv_obj_set_width(_container, lv_pct(100));

    lv_obj_set_height(_container, LV_SIZE_CONTENT);

    lv_obj_set_style_pad_all(_container, 10, 0);

    lv_obj_set_flex_flow(
        _container,
        LV_FLEX_FLOW_COLUMN);


    _title = lv_label_create(_header);

    _name = _title;
    
    lv_obj_set_width(_name, lv_pct(100));
    lv_obj_set_style_text_align(_name, LV_TEXT_ALIGN_LEFT, 0);

    lv_label_set_text(
        _name,
        SimLocalization::translate(stat.id).c_str());

    // Фон полосы

    _bar_bg = lv_obj_create(_container);

    lv_obj_remove_style_all(_bar_bg);

    lv_obj_set_size(_bar_bg, lv_pct(100), 16);

    lv_obj_set_style_radius(
        _bar_bg,
        8,
        0);

    lv_obj_set_style_bg_color(
        _bar_bg,
        lv_color_hex(0x2A2A2A),
        0);

    lv_obj_set_style_border_width(
        _bar_bg,
        0,
        0);

    // Заполнение

    _bar_fill = lv_obj_create(_bar_bg);
    
    
    lv_obj_remove_style_all(_bar_fill);

    int width =
        (220 * stat.value) / stat.max;

    lv_obj_set_size(
        _bar_fill,
        width,
        16);

    lv_obj_align(
        _bar_fill,
        LV_ALIGN_LEFT_MID,
        0,
        0);

    lv_obj_set_style_radius(
        _bar_fill,
        8,
        0);

    lv_obj_set_style_bg_color(
        _bar_fill,
        lv_color_hex(0x39FF14),
        0);

    lv_obj_set_style_border_width(
        _bar_fill,
        0,
        0);
        
    _value = lv_label_create(_header);

    char txt[16];

    snprintf(
        txt,
        sizeof(txt),
        "%d",
        stat.value);

    lv_label_set_text(
        _value,
        txt);
    
    lv_obj_set_style_text_color(
    _value,
    lv_color_hex(0x39FF14),
    0);

    update(stat);

    return true;
}

void StatWidget::update(const Stat &stat)
{
    // Заголовок
    lv_label_set_text(
        _name,
        SimLocalization::translate(stat.id).c_str());

    // Значение
    char txt[16];

    snprintf(
        txt,
        sizeof(txt),
        "%d",
        stat.value);

    lv_label_set_text(
        _value,
        txt);

    // Полоса

    int bgWidth =
        lv_obj_get_width(_bar_bg);

    int width =
        (bgWidth * stat.value) / stat.max;

    if (width < 0)
        width = 0;

    if (width > bgWidth)
        width = bgWidth;

    lv_obj_set_width(
        _bar_fill,
        width);
}