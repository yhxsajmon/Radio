idf_component_register(SRCS "font.c" "screen.c"
                    INCLUDE_DIRS "include" 
                    REQUIRES esp_driver_i2c esp_driver_gpio)
