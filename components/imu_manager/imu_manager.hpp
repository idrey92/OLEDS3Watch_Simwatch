#pragma once

#include "esp_err.h"
#include "qmi8658.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t imu_manager_init(void);

bool imu_manager_read(qmi8658_data_t *data);

bool imu_manager_raise_detected(void);


#ifdef __cplusplus
}
#endif