#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "screen.h"

void powerOnSequence(i2c_master_dev_handle_t oled_handler){
    const uint8_t boot_sequence[] = { 
        0x00,       // Control Byte: invio comandi
        0xA8, 0x3F, // Set MUX Ratio
        0xD3, 0x00, // Set Display Offset
        0x40,       // Set Display Start Line
        0xA1,       // Set Segment re-map
        0xC8,       // Set COM Output Scan Direction
        0x81, 0x7F, // Set Contrast Control
        0xA4,       // Disable Entire Display On
        0xA6,       // Set Normal Display
        0xD5, 0x80, // Set Osc Frequency
        0xDA, 0x02, // Set COM Pins hardware configuration
        0x8D, 0x14, // Abilita Charge Pump
        0xAF,       // Display On
        0xA5        // Forza tutto acceso (test visivo)
    };

    ESP_ERROR_CHECK(i2c_master_transmit(oled_handler, boot_sequence, sizeof(boot_sequence), 1000));
    
}

void oled_init(i2c_master_bus_handle_t master_handler){
    i2c_device_config_t oled_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = OLED_I2C_ADDRESS,
        .scl_speed_hz = 100000 // Frequenza stabile a 100kHz per la breadboard
    };

    i2c_master_dev_handle_t oled_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(master_handler, &oled_config, &oled_handle));

    powerOnSequence(oled_handle);
}