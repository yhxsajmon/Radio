#include <stdio.h>
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "driver/gpio.h"
#include "screen.h"
#include "esp_err.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>




void app_main(void)
{
    i2c_master_bus_handle_t i2cHandler = create_i2c_bus();
    oled_init(i2cHandler);
    printString("");
    // Ciclo infinito pulito con pausa per far respirare la CPU e rispettare il Watchdog
    
}