#include "stat.hpp"

#include <algorithm>

namespace simwatch
{

Stat::Stat()
{
}

Stat::Stat(const StatDefinition& definition)
{
    _definition = definition;
    _runtime.value = definition.initial;
}

const Id& Stat::id() const
{
    return _definition.id;
}

const std::string& Stat::title() const
{
    return _definition.title;
}

const std::string& Stat::icon() const
{
    return _definition.icon;
}

Value Stat::value() const
{
    return _runtime.value;
}

Value Stat::minimum() const
{
    return _definition.minimum;
}

Value Stat::maximum() const
{
    return _definition.maximum;
}

bool Stat::enabled() const
{
    return _definition.enabled;
}

void Stat::setValue(Value value)
{
    _runtime.value = value;

    clamp();

    _runtime.dirty = true;
}

void Stat::add(Value delta)
{
    setValue(_runtime.value + delta);
}

void Stat::subtract(Value delta)
{
    setValue(_runtime.value - delta);
}

const StatDefinition& Stat::definition() const
{
    return _definition;
}

const StatRuntime& Stat::runtime() const
{
    return _runtime;
}

void Stat::clamp()
{
    _runtime.value =
        std::clamp(
            _runtime.value,
            _definition.minimum,
            _definition.maximum);
}

uint32_t Stat::intervalMinutes() const
{
    return _definition.intervalMinutes;
}

Value Stat::passiveDelta() const
{
    return _definition.passiveDelta;
}

void Stat::tick()
{
    add(passiveDelta());
}

void Stat::update(Timestamp elapsedSeconds)
{
    _runtime.accumulatedTime += elapsedSeconds;

    const Timestamp interval =
        static_cast<Timestamp>(_definition.intervalMinutes) * 60;

    if (interval == 0)
        return;

    while (_runtime.accumulatedTime >= interval)
    {
        tick();

        _runtime.accumulatedTime -= interval;
    }
}
}