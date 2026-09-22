#pragma once

#include <stdbool.h>
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "driver/i2c_types.h"

#define OLED_I2C_ADDRESS 0x3C
#define DEFAULT_TIMEOUT 1000
#define STD_SCREEN_ARRAY 1025
#define SEND_DATA_FLAG 0x40


i2c_master_bus_handle_t create_i2c_bus(){};
void oled_init(i2c_master_bus_handle_t master_handler);
void blankScreen();
void printString(char text[]);