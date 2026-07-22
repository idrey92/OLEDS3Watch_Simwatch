#include "engine_test.hpp"

#include "engine.hpp"

#include <cstdio>

namespace simwatch
{

bool runEngineTests()
{
    printf("\n");
    printf("=================================\n");
    printf(" SimWatch Engine Test\n");
    printf("=================================\n");

    Profile profile;

    StatDefinition reading;

    reading.id = "reading";
    reading.title = "Reading";
    reading.initial = 100;
    reading.minimum = 0;
    reading.maximum = 100;
    reading.intervalMinutes = 30;
    reading.passiveDelta = -2;

    profile.stats.push_back(reading);

    Engine engine;

    engine.initialize(profile);

    engine.update(1000);      // первое обновление, только инициализация времени
    engine.update(4600);      // прошло 3600 секунд

    const Stat *stat = engine.findStat("reading");

    if (stat == nullptr)
    {
        printf("FAIL: reading not found\n");
        return false;
    }

    printf("Reading = %.1f\n", stat->value());

    if (stat->value() == 96.0f)
    {
        printf("PASS\n");
        return true;
    }

    printf("FAIL\n");

    return false;
}

}