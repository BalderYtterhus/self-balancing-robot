#include <stdio.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "imu.h"
#include "sensor_fusion.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Self-balancing robot starting");
    esp_err_t err = imu_init();

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "IMU init failed, stopped");
        return;
    }

    err = sensor_fusion_init(200);  // 2 s at 100 Hz; keep the robot still

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Sensor fusion init failed, stopped");
        return;
    }

    imu_data_t data;
    printf("time_us,ax,ay,az,gx,gy,gz,angle\n");
    TickType_t last_wake = xTaskGetTickCount();
    int64_t last_time = esp_timer_get_time();

    while (1) {
        err = imu_read(&data);

        if (err == ESP_OK) {
            int64_t now = esp_timer_get_time();
            float dt = (now - last_time) / 1e6f;  // microseconds → seconds
            last_time = now;

            float angle = sensor_fusion_update(&data, dt);

            printf("%lld,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%.2f\n",
                   now,
                   data.ax, data.ay, data.az,
                   data.gx, data.gy, data.gz,
                   angle);
        } else {
            ESP_LOGW(TAG, "Failed to read data: %s", esp_err_to_name(err));
        }

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10));
    }
}