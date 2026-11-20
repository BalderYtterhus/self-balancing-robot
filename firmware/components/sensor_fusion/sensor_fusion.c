#include "sensor_fusion.h"

#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "sensor_fusion";

#define ALPHA 0.97f                  // chosen in notebook 2 (tau ≈ 0.33 s)
#define RAD_TO_DEG (180.0f / M_PI)
#define CALIBRATION_PERIOD_MS 10     // same rate as the main loop (100 Hz)

static float angle;      // filtered pitch angle, degrees
static float gyro_bias;  // average gy while standing still, degrees/s

/* Pitch angle from the accelerometer alone, in degrees. */
static float accel_angle(const imu_data_t *data)
{
    return atan2f(data->ax, sqrtf(data->ay*data->ay + data->az*data->az))*RAD_TO_DEG;
}

esp_err_t sensor_fusion_init(int samples)
{
    imu_data_t data;
    float sum = 0;

    for (int i = 0; i < samples; i++) {
        esp_err_t err = imu_read(&data);

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read IMU data: %s", esp_err_to_name(err));
            return err;
        }
        sum += data.gy;
        vTaskDelay(pdMS_TO_TICKS(CALIBRATION_PERIOD_MS));
    }
    gyro_bias = sum/(float)samples;
    angle = accel_angle(&data);
    ESP_LOGI(TAG, "Gyro bias: %.2f dps, start angle: %.2f deg", gyro_bias, angle);
    return ESP_OK;
}

float sensor_fusion_update(const imu_data_t *data, float dt)
{
    // TODO: rate = -(gy - gyro_bias)   (gy's sign is opposite to pitch)
    // TODO: angle = ALPHA * (angle + rate * dt) + (1 - ALPHA) * accel_angle
    return angle;
}

float sensor_fusion_get_bias(void)
{
    return gyro_bias;
}
