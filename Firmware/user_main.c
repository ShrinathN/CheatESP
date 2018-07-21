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
//#define DEBUG_ENABLE 0

#include "user_config.h"
#include "timerkeeping.h"
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
    Oled_eraseScreen();
    Oled_returnCursor();
    setupInterrupt();
    SetupNetwork();
}

//==========ENTRY POINT==========
void user_init(void)
{
#ifdef DEBUG_ENABLE
    SET_BAUD(0,115200); //sets the BAUD rate to 115200
#else
    system_uart_swap();
#endif
    initFunction();
}
