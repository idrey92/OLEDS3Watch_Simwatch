#pragma once

#include <vector>

#include "sim_stat.hpp"

namespace simwatch
{

class SimCore
{
public:

    bool begin();

    void update();

    bool addStat(const std::string& id,
                int value,
                int min,
                int max,
                int intervalMinutes,
                int delta,
                bool enabled = true);

    std::vector<Stat>& stats();

private:

    std::vector<Stat> _stats;
};

}