idf_component_register(SRCS "sensor_fusion.c"
                       INCLUDE_DIRS "include"
                       REQUIRES imu
                       PRIV_REQUIRES freertos)
