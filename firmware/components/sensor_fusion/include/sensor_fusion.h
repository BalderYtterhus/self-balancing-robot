#pragma once

#include "esp_err.h"
#include "imu.h"

/*
 * Complementary filter for the robot's pitch angle (tilt forward/back).
 * Values from experiments/notebooks/02_sensor_fusion.ipynb.
 */

/*
 * Measure the gyro bias by averaging `samples` IMU readings while the robot
 * stands still, and start the angle at the accelerometer's angle.
 * Call once at startup, after imu_init().
 */
esp_err_t sensor_fusion_init(int samples);

/*
 * Update the filter with one IMU reading taken `dt` seconds after the last.
 * Returns the filtered pitch angle in degrees.
 */
float sensor_fusion_update(const imu_data_t *data, float dt);

/* The gyro bias measured by sensor_fusion_init(), in degrees per second. */
float sensor_fusion_get_bias(void);
