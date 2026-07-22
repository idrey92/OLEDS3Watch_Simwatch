#pragma once

#include <vector>

#include "stat_definition.hpp"

namespace simwatch
{

struct Profile
{
    std::vector<StatDefinition> stats;
};

}