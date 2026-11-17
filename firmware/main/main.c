#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "imu.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Self-balancing robot starting");
    imu_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
