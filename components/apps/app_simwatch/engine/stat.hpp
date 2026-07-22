#pragma once

#include "stat_definition.hpp"
#include "stat_runtime.hpp"

namespace simwatch
{

class Stat
{
public:

    Stat();

    explicit Stat(const StatDefinition& definition);

    const Id& id() const;

    const std::string& title() const;

    const std::string& icon() const;

    Value value() const;

    Value minimum() const;

    Value maximum() const;

    bool enabled() const;

    void setValue(Value value);

    void add(Value delta);

    void subtract(Value delta);

    const StatDefinition& definition() const;

    const StatRuntime& runtime() const;
    
    uint32_t intervalMinutes() const;

    Value passiveDelta() const;

    void tick();

    void update(Timestamp elapsedSeconds);

private:

    void clamp();

    

private:

    StatDefinition _definition;

    StatRuntime _runtime;
};

}