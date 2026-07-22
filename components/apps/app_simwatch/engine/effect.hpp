#pragma once

#include "types.hpp"

namespace simwatch
{

enum class EffectType
{
    Passive,
    Activity,
    Rule,
    Reward,
    Economy,
    Notification,
    Debug
};

struct Effect
{
    Id target;

    Value delta = 0;

    Timestamp timestamp = 0;

    Id source;

    EffectType type = EffectType::Passive;
};

}