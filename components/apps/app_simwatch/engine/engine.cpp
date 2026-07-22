#include "engine.hpp"

namespace simwatch
{

bool Engine::initialize(const Profile& profile)
{
    _profile = profile;

    _stats.clear();

    _index.clear();

    _stats.reserve(profile.stats.size());

    for (const auto& definition : profile.stats)
    {
        _stats.emplace_back(definition);

        _index.emplace(
            _stats.back().id(),
            _stats.size() - 1);
    }

    _lastUpdate = 0;
    
    return true;
}

bool Engine::update(Timestamp now)
{
    if (_lastUpdate == 0)
    {
        _lastUpdate = now;
        return true;
    }

    const Timestamp elapsed = now - _lastUpdate;

    for (auto& stat : _stats)
    {
        stat.update(elapsed);
    }

    _lastUpdate = now;

    return true;
}

const std::vector<Stat>& Engine::stats() const
{
    return _stats;
}

Stat* Engine::findStat(const Id& id)
{
    auto it = _index.find(id);

    if (it == _index.end())
        return nullptr;

    return &_stats[it->second];
}

const Stat* Engine::findStat(const Id& id) const
{
    auto it = _index.find(id);

    if (it == _index.end())
        return nullptr;

    return &_stats[it->second];
}

}