#pragma once

#include <string>
#include <stdint.h>

namespace simwatch
{

class Stat
{
public:

    // Уникальный идентификатор
    std::string id;

    // Название (пока на английском)
    std::string title;

    // LVGL символ
    std::string icon;

    // Цвет прогресс-бара
    uint32_t color = 0x39FF14;

    // Значение
    int value = 100;

    // Границы
    int min = 0;
    int max = 100;

    // Настройки изменения
    int intervalMinutes = 60;
    int delta = -1;

    bool enabled = true;

    uint64_t lastUpdate = 0;
};

}