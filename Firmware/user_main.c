// main file with entry point

/*  Copyright (C) 2018 Shrinath Nimare
    This file is part of CheatESP
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//uncomment to enable debug
#define DEBUG_ENABLE 0

#include "user_config.h"
#include "i2c.h"
#include "oled.h"
#include "fonts.h"
#include "oled_functions.h"
#include "protocol.h"
#include "interface.h"
#include "interrupt_config.h"
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)

void ICACHE_FLASH_ATTR
initFunction()
{
    i2c_init();
    Oled_init();
    SetupNetwork();
}

//==========ENTRY POINT==========
void user_init(void)
{
    SET_BAUD(0,115200); //sets the BAUD rate to 115200
    initFunction();
}
//===============================

//==========FOR REFERENCE==========
/*
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
*/
