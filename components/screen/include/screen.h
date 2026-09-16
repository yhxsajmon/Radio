#pragma once

#include <stdbool.h>
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"

#define OLED_I2C_ADDRESS 0x3C



void oled_init(i2c_master_bus_handle_t master_handler);
void powerOnSequence(i2c_master_dev_handle_t oled_handler);
