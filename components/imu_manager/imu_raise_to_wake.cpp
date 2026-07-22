#include "imu_raise_to_wake.hpp"

#include <math.h>

static bool movement = false;
static uint32_t movementTime = 0;

void imu_raise_to_wake_init()
{
    movement = false;
    movementTime = 0;
}

bool imu_raise_to_wake_update(
    float ax,
    float ay,
    float az,
    float gx,
    float gy,
    float gz)
{
    float gyro = fabsf(gx) + fabsf(gy) + fabsf(gz);

    if (!movement)
    {
        if (gyro > 150.0f)
        {
            movement = true;
            movementTime = 0;
        }

        return false;
    }

    movementTime += 20;

    if (movementTime > 600)
    {
        movement = false;
        return false;
    }

    if (az < -850.0f)
    {
        movement = false;
        return true;
    }

    return false;
}