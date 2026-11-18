#include "imu.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#define SDA_PIN GPIO_NUM_7
#define SCL_PIN GPIO_NUM_5
#define SENSOR_ADDRESS 0x69
#define I2C_TIMEOUT_MS 50
#define WHO_AM_I 0x00 // Identity
#define WHO_AM_I_VALUE 0xEA
#define REG_PWR_MGMT_1 0x06 // Power control
#define REG_ACCEL_XOUT_H 0x2D //Accelerometer bytes
#define REG_GYRO_XOUT_H 0x33 // Gyrometer bytes
#define REG_BANK_SEL 0x7F // Selects register bank
#define PWR_MGMT_1_WAKE 0x01

static const char *TAG = "imu";
static i2c_master_bus_handle_t bus_handle;
static i2c_master_dev_handle_t dev_handle;

static esp_err_t imu_read_reg(uint8_t reg, uint8_t *buf, size_t len){
    esp_err_t call = i2c_master_transmit_receive(dev_handle, &reg, 1, buf, len, I2C_TIMEOUT_MS);
    return call;
}

static esp_err_t imu_write_reg(uint8_t reg, uint8_t value){
    uint8_t buf[2]  = {reg, value};
    esp_err_t result = i2c_master_transmit(dev_handle, buf, sizeof(buf), I2C_TIMEOUT_MS);
    return result;
}

esp_err_t imu_init(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = -1,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };

    esp_err_t err = i2c_new_master_bus(&bus_config, &bus_handle);

    if (err != ESP_OK){
        ESP_LOGE(TAG, "Failed to create I2C bus: %s", esp_err_to_name(err));
        return err;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7, 
        .device_address = SENSOR_ADDRESS,
        .scl_speed_hz = 100000,
    };

    err = i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Failed to add device to bus: %s", esp_err_to_name(err));
        return err;
    }

    uint8_t response;

    err = imu_read_reg(WHO_AM_I, &response, 1);

    if (err != ESP_OK){
        ESP_LOGE(TAG, "Failed to receive data: %s", esp_err_to_name(err));
        return err;
    }
    if (response != WHO_AM_I_VALUE){
        ESP_LOGE(TAG, "Unknown data: 0x%02X", response);
        return ESP_ERR_NOT_FOUND;
    }
    ESP_LOGI(TAG, "WHO_AM_I: 0x%02X", response);
    return ESP_OK;
}

esp_err_t imu_read(imu_data_t *data){
    return ESP_ERR_NOT_SUPPORTED;
}