#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void imu_raise_to_wake_init(void);

bool imu_raise_to_wake_update(
    float ax,
    float ay,
    float az,
    float gx,
    float gy,
    float gz);

#ifdef __cplusplus
}
#endif