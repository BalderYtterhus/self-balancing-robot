#include <stdio.h>
#include "imu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"


#define SDA_PIN GPIO_NUM_7
#define SCL_PIN GPIO_NUM_5
#define SENSOR_ADDRESS 0x69
#define timeout 50

void app_main(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = -1,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };

    i2c_master_bus_handle_t bus_handle; // Referanse til hvor bussen ligger i minnet, nyttig hvis vi vil gjøre ting med bussen
    i2c_new_master_bus(&bus_config, &bus_handle); // Lager en ny bus

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7, 
        .device_address = SENSOR_ADDRESS,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle);

    uint8_t register_address = 0x00;
    uint8_t response;

    i2c_master_transmit_receive(
        dev_handle, 
        &register_address, 
        1,
        &response, 
        1, 
        50
    );

    esp_err_t result = i2c_master_transmit_receive(dev_handle, &register_address, 1,&response, 1, 50);

    if (result == ESP_OK) {
        printf("WHO AM I: 0x%02X\n", response);
    }
}