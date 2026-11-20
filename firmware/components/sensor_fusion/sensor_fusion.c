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
    // TODO: atan2(ax, sqrt(ay² + az²)), converted to degrees
    return 0.0f;
}

esp_err_t sensor_fusion_init(int samples)
{
    // TODO: read the IMU `samples` times, CALIBRATION_PERIOD_MS apart,
    //       and add up gy. Return the error if a read fails.
    // TODO: gyro_bias = average of gy
    // TODO: start the angle at the accelerometer angle of the last reading
    // TODO: log the bias and the starting angle
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
