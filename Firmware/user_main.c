#include "user_config.h"
#include "protocol.h"
#include "i2c.h"
#include "oled.h"
#include "fonts.h"
#include "oled_functions.h"
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)

void ICACHE_FLASH_ATTR exampleHelloFunction();

//string to print
uint8 stringToWrite[] = {0x7, 0x4, 0xb, 0xb, 0xe, 0x2a, 0x16, 0xe, 0x11, 0xb, 0x3,
                         0x2a, 0x13, 0x7, 0x8, 0x12, 0x2a, 0x8, 0x12, 0x2a, 0xc,
                         0x18, 0x2a, 0x13, 0x4, 0x12, 0x13, 0x2a, 0xc, 0x4, 0x12,
                         0x12, 0x0, 0x6, 0x4, 0x2a, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e};
uint8 newString[] = {0,1,2,3,4,5,6,7,8,9};

void user_init(void)
{
    SET_BAUD(0,115200); //sets the BAUD rate to 115200
    os_delay_us(50000);
    exampleHelloFunction(); //calls the example function
}

void ICACHE_FLASH_ATTR exampleHelloFunction()
{
    i2c_init(); //I2C init function
    Oled_init(); //OLED init function
    Oled_eraseScreen(); //function to erase
    Oled_writeString(stringToWrite, sizeof(stringToWrite)); //function to write the string
    Oled_returnCursor();
    Oled_writeString(newString, sizeof(newString)); //function to write the string
}
