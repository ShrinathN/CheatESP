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

#include "user_config.h"
#include "timerkeeping.h"
#include "i2c.h"
#include "oled.h"
#include "fonts.h"
#include "oled_functions.h"
#include "network.h"
#include "interface.h"
#include "interrupt_config.h"

void ICACHE_FLASH_ATTR
initFunction()
{
    i2c_init();
    OledFunctions_init();
    OledFunctions_eraseScreen();
    OledFunctions_returnCursor();
    InterruptConfig_setupInterrupt();
    Network_SetupNetwork();
}

void ICACHE_FLASH_ATTR
itwerk()
{
    OledStringStruct *p = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
    OledStringPtr *pt = (OledStringPtr*)os_zalloc(7);
    OledFunctions_stringToOledString("itworks", pt);
    p->ptr = pt;
    p->len = 7;
    OledFunctions_eraseScreen();
    OledFunctions_returnCursor();
    OledFunctions_writeString(p);
    os_free(p);
    os_free(pt);
}

void ICACHE_FLASH_ATTR
timeFunction()
{
    OledStringStruct * temp = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
    uint8 length = (uint8)os_strlen(sntp_get_real_time(realTime));
    OledStringPtr * pt = (OledStringPtr *)os_zalloc(length);
    OledFunctions_stringToOledString(sntp_get_real_time(realTime), pt);
    temp->len = length;
    temp->ptr = pt;
    OledFunctions_writeString(temp);
    os_free(temp);
    os_free(pt);
}

//==========ENTRY POINT==========
void user_init(void)
{
    system_uart_swap();
    initFunction();
    static OledStringPtr spa[13] = {0x0f, 0x11, 0x08, 0x0d, 0x13, 0x2a, 0x08, 0x13, 0x16, 0x0e, 0x11, 0x0a, 0x12};
    static OledStringPtr spb[20] = {0x03, 0x08, 0x12, 0x0f, 0x0b, 0x00, 0x18, 0x2a, 0x02, 0x14, 0x11, 0x11, 0x04, 0x0d, 0x13, 0x2a, 0x13, 0x08, 0x0c, 0x04};
    static OledStringPtr spc[6] = {8,19,4,12,42,29};
    static OledStringStruct a;
    static OledStringStruct b;
    static OledStringStruct c;
    a.ptr = spa;
    a.len = 13;
    b.ptr = spb;
    b.len = 20;
    c.ptr = spc;
    c.len = 6;
    MenuStruct *menuStruct;
    menuStruct = (MenuStruct *)os_zalloc(sizeof(MenuStruct));
    menuStruct->totalElements = 3;
    menuStruct->currentElement = 2;
    OledFunctions_optionSet(menuStruct, 0, itwerk, &a);
    OledFunctions_optionSet(menuStruct, 1, timeFunction, &b);
    OledFunctions_optionSet(menuStruct, 2, NULL, &c);
    OledFunctions_setGlobalMenu(menuStruct);
}
