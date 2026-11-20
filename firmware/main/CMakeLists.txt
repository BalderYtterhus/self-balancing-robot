idf_component_register(
                        SRCS "main.c"
                        PRIV_REQUIRES imu sensor_fusion esp_timer
                        INCLUDE_DIRS "."
                        )
