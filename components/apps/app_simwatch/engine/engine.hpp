#pragma once

#include <vector>
#include <unordered_map>
#include "profile.hpp"
#include "stat.hpp"


namespace simwatch
{

class Engine
{
public:

    bool initialize(const Profile& profile);

    bool update(Timestamp now);

    const std::vector<Stat>& stats() const;

    Stat* findStat(const Id& id);

    const Stat* findStat(const Id& id) const;

private:

    Profile _profile;

    std::vector<Stat> _stats;

    std::unordered_map<Id, size_t> _index;

    Timestamp _lastUpdate = 0;
};

}