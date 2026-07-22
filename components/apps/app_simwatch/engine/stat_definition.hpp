#pragma once

#include <string>

#include "types.hpp"

namespace simwatch
{

struct StatDefinition
{
    Id id;

    std::string title;

    std::string icon;

    Value minimum = 0;

    Value maximum = 100;

    Value initial = 100;

    Value passiveDelta = 0;

    uint32_t intervalMinutes = 60;

    bool enabled = true;
};

}