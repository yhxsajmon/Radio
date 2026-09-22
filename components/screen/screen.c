#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "screen.h"
#include "font.h"
#include <string.h>



void chessboard();
void reverseChessboard();
void powerOnSequence();


static i2c_master_dev_handle_t oled_handler = NULL;


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


/**
 * @brief Funzione di scrittura su riga
 * @ return NULL
 * @note TODO: Switch to dynamic memory allocation, and add a row system to write on each Page
*/ 
void printString(char text[]){
  //Dimensione temporanea del buffer
    uint8_t package[1024] = { SEND_DATA_FLAG };
    int endPointer = 1;
    uint8_t buffer[5];
  //Costruzione del pacchetto 
    for(size_t i = 0; i < strlen(text); i++){
    //Estrazione del carattere
        if(text[i] > 31 && text[i] < 128){
        for(size_t j = 0;j < 5;j++){
            buffer[j] = font[text[i] - 32][j];
        }
        
      //Aggiunta del buffer sull'array 
        for(size_t k = endPointer; k < endPointer + 5; k++){
            package[k] = buffer[k - endPointer];
        }
      //sistemare posizioen endPointer
        endPointer = endPointer +5; 
        }
    }  
    ESP_ERROR_CHECK(i2c_master_transmit(oled_handler,package,sizeof(package),DEFAULT_TIMEOUT));
}



/**
 * @brief Inizializes the I2C connection
 * 
 * @param master_handler master I2C handle
 * @return ESP_OK se il pin è valido, ESP_FAIL altrimenti.
 * @note None.
 */
void oled_init(i2c_master_bus_handle_t master_handler){
    i2c_device_config_t oled_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = OLED_I2C_ADDRESS,
        .scl_speed_hz = 400000 // Frequenza stabile a 100kHz per la breadboard
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(master_handler, &oled_config, &oled_handler));
    powerOnSequence();
}


/**
 * @brief Helper function for first time screen power on
 */
void powerOnSequence(){
    const uint8_t boot_sequence[] = { 
        0x00,       
        0xA8, 0x3F, 
        0xD3, 0x00,
        SEND_DATA_FLAG,      
        0xA1,      
        0xC8,       
        0x81, 0x7F, 
        0xA4,      
        0xA6,       
        0xD5, 0x80, 
        0xDA, 0x02, 
        0x8D, 0x14, 
        0xAF,     
    };
    ESP_ERROR_CHECK(i2c_master_transmit(oled_handler, boot_sequence, sizeof(boot_sequence), DEFAULT_TIMEOUT));

    blankScreen();
    vTaskDelay(DEFAULT_TIMEOUT);
}


/** 
 * @brief Testing function to display a checked board on the screen
*/
void chessboard() {
    printf("Printing the chessboard\n");
    
    // Allocazione di 1025 byte (1 byte comando + 1024 byte dati per schermo 128x64)
    uint8_t* checkboardArray = malloc(sizeof(uint8_t) * 1025);

    if (checkboardArray != NULL) {
        checkboardArray[0] = SEND_DATA_FLAG; // Byte di controllo per i dati (Co=0, D/C=1)

        // Iteriamo sulle 8 "pagine" orizzontali (ogni pagina è alta 8 pixel)
        for (size_t page = 0; page < 8; page++) {
            // Iteriamo sulle 128 colonne di ogni pagina
            for (size_t col = 0; col < 128; col++) {
                size_t index = 1 + (page * 128) + col;

                // Calcoliamo se il quadrato 8x8 è bianco o nero
                // (col / 8) fornisce la coordinata X del blocco
                // page fornisce la coordinata Y del blocco
                if (((col / 8) + page) % 2 == 0) {
                    checkboardArray[index] = 0xFF; // Quadrato acceso
                } else {
                    checkboardArray[index] = 0x00; // Quadrato spento
                }
            }
        }

        // La trasmissione e il free vanno FUORI dal ciclo di generazione
        ESP_ERROR_CHECK(i2c_master_transmit(oled_handler, checkboardArray, 1025, DEFAULT_TIMEOUT));
        free(checkboardArray);
        
    } else {
        printf("Errore: allocazione memoria fallita\n");
    }
}

void blankScreen(){
    uint8_t* empty = malloc(sizeof(uint8_t)*1026);
    uint8_t horizontal[] = {0x00 ,0x20, 0x00};
    if(empty!= NULL){
        empty[0] = SEND_DATA_FLAG;
        
        for(size_t i=1;i<1026;i++){
            empty[i] = 0x00;
        }

        ESP_ERROR_CHECK(i2c_master_transmit(oled_handler, horizontal, sizeof(horizontal), DEFAULT_TIMEOUT));
        ESP_ERROR_CHECK(i2c_master_transmit(oled_handler, empty, 1026, DEFAULT_TIMEOUT));
        free(empty);
    }
}
