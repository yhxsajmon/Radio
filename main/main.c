#include <stdio.h>
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "driver/gpio.h"
#include "screen.h"
#include "esp_err.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SDA_PIN GPIO_NUM_6
#define SCL_PIN GPIO_NUM_5

i2c_master_bus_handle_t create_i2c_bus(){
    i2c_master_bus_config_t i2c_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = -1,
        .scl_io_num = SCL_PIN,
        .sda_io_num = SDA_PIN,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t busHandler;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &busHandler));

    return busHandler;
}

void app_main(void)
{
    i2c_master_bus_handle_t i2cHandler = create_i2c_bus();
    oled_init(i2cHandler);

    // Ciclo infinito pulito con pausa per far respirare la CPU e rispettare il Watchdog
    while(true){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}