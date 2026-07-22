#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "imu_manager.hpp"
#include "display_manager.h"
#include "bsp/display.h"
#include "bsp/esp32_s3_touch_amoled_2_06.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "settings.h"
//#include "nimble-nordic-uart.h"
// Power management
#include "sdkconfig.h"
#include "esp_sleep.h"
#if CONFIG_PM_ENABLE
#include "esp_pm.h"
#endif


// If the board provides simple GPIO buttons, use one as wake key.
// On this hardware BSP_CAPS_BUTTONS is 0, so we will use the PMU PWR key instead.
#define DISPLAY_BUTTON GPIO_NUM_0
//#define CONFIG_PMU_INTERRUPT_PIN 35

static const char *TAG = "DISPLAY_MGR";

static bool display_on = true;
static volatile bool screen_was_woken = false;
static uint32_t timeout_ms;
#if CONFIG_PM_ENABLE
static esp_pm_lock_handle_t s_no_ls_lock = NULL;
#endif

static void display_turn_off_internal(void) {
    if (!display_on) {
        return;
    }
    ESP_LOGI(TAG, "Turning display off");

bsp_display_brightness_set(0);

lv_indev_t *indev = bsp_display_get_input_dev();
if (indev)
{
    lv_indev_enable(indev, false);
}

display_on = false;
}



void display_manager_turn_off(void) {
    display_turn_off_internal();
}

void display_manager_turn_on(void) {
    if (!display_on) 
    {
        ESP_LOGI(TAG, "Turning display on");
         
        // Wake the panel first, then resume LVGL and restore brightness        
        bsp_display_brightness_set(settings_get_brightness());

        lv_indev_t *indev = bsp_display_get_input_dev();
        if (indev)
        {
            lv_indev_enable(indev, true);
        }

        display_on = true;

        screen_was_woken = true;

        display_manager_reset_timer();
    }
    // Prevent light sleep while actively displaying UI for responsiveness
#if CONFIG_PM_ENABLE
    if (s_no_ls_lock) {
        (void)esp_pm_lock_acquire(s_no_ls_lock);
    }
#endif
    // Restore more responsive BLE params when screen is on
    //nordic_uart_set_low_power_mode(false);
    display_manager_reset_timer();
}

bool display_manager_is_on(void) {
    return display_on;
}

void display_manager_reset_timer(void) {
    lv_disp_trig_activity(NULL);
}

static void touch_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
        case LV_EVENT_PRESSED:
        case LV_EVENT_PRESSING:
        case LV_EVENT_RELEASED:
        case LV_EVENT_CLICKED:
        case LV_EVENT_LONG_PRESSED:
        case LV_EVENT_LONG_PRESSED_REPEAT:
        case LV_EVENT_GESTURE:
            display_manager_reset_timer();
            break;
        default:
            break; // ignore non-input/render events
    }
}

static bool wake_button_pressed(void)
{
    return gpio_get_level(GPIO_NUM_0) == 0;
}

static void display_manager_task(void *arg) {
    ESP_LOGI(TAG, "Display manager task started");
    while (1)
{
    timeout_ms = settings_get_display_timeout();

    if (display_on)
    {
        if (!settings_get_always_on())
        {
            uint32_t inactive = lv_disp_get_inactive_time(NULL);

            if (inactive >= timeout_ms)
            {
                display_turn_off_internal();
            }
        }
    }

    // Проверяем кнопку ВСЕГДА
    if (wake_button_pressed())
    {
        ESP_LOGI(TAG, "Wake button");

        if (!display_on)
        {
            display_manager_turn_on();
        }

        display_manager_reset_timer();

        vTaskDelay(pdMS_TO_TICKS(200));
    }

    if (!display_on)
{
    if (imu_manager_raise_detected())
    {
        ESP_LOGI(TAG, "Raise to Wake");

        display_manager_turn_on();
    }
}
    vTaskDelay(pdMS_TO_TICKS(50));
}
}

void display_manager_init(void) {
    timeout_ms = settings_get_display_timeout();

    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << DISPLAY_BUTTON,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    bsp_display_brightness_set(settings_get_brightness());

    xTaskCreate(display_manager_task, "display_mgr", 4000, NULL, 4, NULL);
}

bool display_manager_was_woken(void)
{
    return screen_was_woken;
}

void display_manager_clear_woken(void)
{
    screen_was_woken = false;
}

void display_manager_pm_early_init(void)
{
#if CONFIG_PM_ENABLE
    if (!s_no_ls_lock) {
        (void)esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, "display", &s_no_ls_lock);
    }
    if (s_no_ls_lock) {
        (void)esp_pm_lock_acquire(s_no_ls_lock);
    }
#else
    // No power management; nothing to do
    (void)0;
#endif
}
