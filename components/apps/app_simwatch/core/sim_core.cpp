#include "sim_core.hpp"

namespace simwatch
{

bool SimCore::begin()
{
    _stats.clear();

    addStat(
    "hunger",
    100,
    0,
    100,
    120,
    -3
    );

    addStat(
    "sleep",
    100,
    0,
    100,
    60,
    -2
    );

    addStat(
        "energy",
        100,
        0,
        100,
        90,
        -1
    );

    addStat(
        "mood",
        100,
        0,
        100,
        180,
        -1
    );

    return true;
}

void SimCore::update()
{
}

bool SimCore::addStat(
    const std::string& id,
    int value,
    int min,
    int max,
    int intervalMinutes,
    int delta,
    bool enabled)
{
    Stat stat;

    stat.id = id;
    stat.title = id;
    stat.value = value;
    stat.min = min;
    stat.max = max;
    stat.intervalMinutes = intervalMinutes;
    stat.delta = delta;
    stat.enabled = enabled;

    _stats.push_back(stat);

    return true;
}



std::vector<Stat>& SimCore::stats()
{
    return _stats;
}

}