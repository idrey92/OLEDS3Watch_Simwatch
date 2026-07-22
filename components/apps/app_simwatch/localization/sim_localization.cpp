#include "sim_localization.hpp"

namespace simwatch
{

std::string SimLocalization::translate(const std::string &id)
{
    if (id == "hunger")
        return "Hunger";

    if (id == "sleep")
        return "Sleep";

    if (id == "energy")
        return "Energy";

    if (id == "money")
        return "Money";

    if (id == "mood")
        return "Mood";

    return id;
}

}