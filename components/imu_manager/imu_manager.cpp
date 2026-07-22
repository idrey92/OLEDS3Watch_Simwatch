#include "imu_manager.hpp"

#include "bsp/esp32_s3_touch_amoled_2_06.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <math.h>

static const char *TAG = "IMU";

static qmi8658_dev_t imu;

static volatile bool raise_detected = false;

enum RaiseState
{
    WAIT_ARM_DOWN,
    WAIT_MOVEMENT,
    WAIT_VIEW_POSITION
};

static RaiseState raiseState = WAIT_ARM_DOWN;
static uint32_t raiseTimer = 0;

static bool waiting_orientation = false;
static uint32_t timer = 0;


static void imu_test_task(void *arg)
{
    qmi8658_data_t data;

    while (1)
    {
        if (imu_manager_read(&data))
        {
            ESP_LOGI(TAG,
                     "ACC: %7.2f %7.2f %7.2f   GYR: %7.2f %7.2f %7.2f",
                     data.accelX,
                     data.accelY,
                     data.accelZ,
                     data.gyroX,
                     data.gyroY,
                     data.gyroZ);
            float gyro =
            fabsf(data.gyroX) +
            fabsf(data.gyroY) +
            fabsf(data.gyroZ);

            switch (raiseState)
            {
            case WAIT_ARM_DOWN:

                // Ждем, пока рука опущена вниз
                if (data.accelY < -600)
                {
                    raiseState = WAIT_MOVEMENT;
                }

                break;

            case WAIT_MOVEMENT:

                // Если руку подняли обратно — начать заново
                if (data.accelY > -400)
                {
                    raiseState = WAIT_ARM_DOWN;
                    break;
                }

                // Обнаружили движение
                if (gyro > 180)
                {
                    raiseState = WAIT_VIEW_POSITION;
                    raiseTimer = 0;
                }

                break;

            case WAIT_VIEW_POSITION:

                raiseTimer += 20;

                // Таймаут
                if (raiseTimer > 800)
                {
                    raiseState = WAIT_ARM_DOWN;
                    break;
                }

                // Часы повернулись к лицу
                if (data.accelY > 150)
                {
                    raise_detected = true;
                    raiseState = WAIT_ARM_DOWN;
                }

                break;
            }  
        }
        else
        {
            ESP_LOGE(TAG, "Read failed");
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

esp_err_t imu_manager_init(void)
{
    esp_err_t err = qmi8658_init(
        &imu,
        bsp_i2c_get_handle(),
        QMI8658_ADDRESS_HIGH);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "QMI8658 init failed (%s)", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "QMI8658 initialized");

    xTaskCreate(
        imu_test_task,
        "imu_test",
        4096,
        NULL,
        5,
        NULL);

    return ESP_OK;
}

bool imu_manager_read(qmi8658_data_t *data)
{
    return qmi8658_read_sensor_data(&imu, data) == ESP_OK;
}

bool imu_manager_raise_detected(void)
{
    if (raise_detected)
    {
        raise_detected = false;
        return true;
    }

    return false;
}