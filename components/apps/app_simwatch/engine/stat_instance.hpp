#pragma once

#include "stat_definition.hpp"
#include "stat_runtime.hpp"

namespace simwatch
{

struct StatInstance
{
    StatDefinition definition;

    StatRuntime runtime;
};

}