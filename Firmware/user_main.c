#include "user_config.h"
#include "protocol.h"
#include "i2c.h"
#include "oled.h"
#include "fonts.h"
#include "oled_functions.h"
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)

void ICACHE_FLASH_ATTR exampleHelloFunction();

//string to print
uint8 stringToWrite[13] = {0x07,0x04,0x0b,0x0b,0x0e,43,43,43,0x07,0x04,0x0b,0x0b,0x0e};

void user_init(void)
{
    SET_BAUD(0,115200); //sets the BAUD rate to 115200
    exampleHelloFunction(); //calls the example function
}

void ICACHE_FLASH_ATTR exampleHelloFunction()
{
    i2c_init(); //I2C init function
    Oled_init(); //OLED init function
    Oled_eraseScreen(); //function to erase
    Oled_writeString(stringToWrite, sizeof(stringToWrite)); //function to write the string
}
