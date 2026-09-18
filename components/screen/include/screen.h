#pragma once

#include <stdbool.h>
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"

#define OLED_I2C_ADDRESS 0x3C
#define DEFAULT_TIMEOUT 1000
#define STD_SCREEN_ARRAY 1025;

void oled_init(i2c_master_bus_handle_t master_handler);
void blankScreen();
