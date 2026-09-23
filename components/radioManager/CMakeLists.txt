idf_component_register(SRCS "radioManager.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_hal_gpspi esp_driver_spi 
)
