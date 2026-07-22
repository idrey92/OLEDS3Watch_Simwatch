#pragma once

#include "types.hpp"

namespace simwatch
{

struct StatRuntime
{
    Value value = 0;

    Timestamp lastUpdate = 0;

    Timestamp accumulatedTime = 0;

    bool dirty = false;
};


}