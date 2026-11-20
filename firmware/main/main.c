#include <stdio.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "imu.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Self-balancing robot starting");
    esp_err_t err = imu_init();

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "IMU init failed, stopped");
        return;
    }

    imu_data_t data;
    printf("time_us,ax,ay,az,gx,gy,gz\n");
    TickType_t last_wake = xTaskGetTickCount();

    while (1) {
        err = imu_read(&data);
        if (err == ESP_OK) {
            printf("%lld,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f\n",
                   esp_timer_get_time(),
                   data.ax, data.ay, data.az,
                   data.gx, data.gy, data.gz);
        } else {
            ESP_LOGW(TAG, "Failed to read data: %s", esp_err_to_name(err));
        }

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10));
    }
}
