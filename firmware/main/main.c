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

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
