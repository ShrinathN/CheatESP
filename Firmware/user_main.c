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
#include "libs/timerkeeping.h"
#include "libs/i2c.h"
#include "libs/oled.h"
#include "libs/fonts.h"
#include "libs/oled_functions.h"
#include "libs/network.h"
#include "libs/interface.h"
#include "libs/interrupt_config.h"

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
chosen_Next()
{
    OledStringStruct *menuItem1 = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
    menuItem1->len = (unsigned char)os_strlen("short press");
    menuItem1->ptr = (OledStringPtr *)os_zalloc(menuItem1->len);
    OledFunctions_stringToOledString("short press", menuItem1->ptr);
    OledFunctions_writeString(menuItem1);
    os_free(menuItem1->ptr);
    os_free(menuItem1);
}

void ICACHE_FLASH_ATTR
chosen_Select()
{
    OledStringStruct *menuItem1 = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
    menuItem1->len = (unsigned char)os_strlen("long press");
    menuItem1->ptr = (OledStringPtr *)os_zalloc(menuItem1->len);
    OledFunctions_stringToOledString("long press", menuItem1->ptr);
    OledFunctions_writeString(menuItem1);
    os_free(menuItem1->ptr);
    os_free(menuItem1);
}

void ICACHE_FLASH_ATTR
second_stage_function()
{
    Interface_setNextPointer(chosen_Next);
    Interface_setSelectPointer(chosen_Select);
}

void ICACHE_FLASH_ATTR
first_stage_select()
{
    MenuStruct * menu = (MenuStruct *)os_zalloc(sizeof(MenuStruct));
    menu->currentElement = 0;
    menu->totalElements = 0;

    //will store "new menu yay"
    static OledStringStruct menuItem1;
    menuItem1.len = (unsigned char)os_strlen("new menu yay");
    menuItem1.ptr = (OledStringPtr *)os_zalloc(menuItem1.len);
    OledFunctions_stringToOledString("new menu yay", menuItem1.ptr);
    menu->totalElements++;

    //will store "select me plsplspls"
    static OledStringStruct menuItem2;
    menuItem2.len = (unsigned char)os_strlen("select me plsplspls");
    menuItem2.ptr = (OledStringPtr *)os_zalloc(menuItem2.len);
    OledFunctions_stringToOledString("select me plsplspls", menuItem2.ptr);
    menu->totalElements++;

    OledFunctions_optionSet(menu,0,NULL,&menuItem1);
    OledFunctions_optionSet(menu,1,second_stage_function,&menuItem2);
    OledFunctions_setGlobalMenu(menu);
    OledFunctions_drawGlobalMenu();
}

void user_init(void)
{
    //initialization
    system_uart_swap();
    initFunction();

    //for default menu behavior
    Interface_setNextPointer(Interface_defaultNext);
    Interface_setSelectPointer(Interface_defaultSelect);

    //menu struct being declared and allocated memory
    MenuStruct * menu = (MenuStruct *)os_zalloc(sizeof(MenuStruct));
    menu->currentElement = 0;
    menu->totalElements = 0;

    //will store "hello world item 1"
    static OledStringStruct menuItem1;
    menuItem1.len = (unsigned char)os_strlen("hello world item 1");
    menuItem1.ptr = (OledStringPtr *)os_zalloc(menuItem1.len);
    OledFunctions_stringToOledString("hello world item 1", menuItem1.ptr);
    menu->totalElements++;

    //will store "select me"
    static OledStringStruct menuItem2;
    menuItem2.len = (unsigned char)os_strlen("select me");
    menuItem2.ptr = (OledStringPtr *)os_zalloc(menuItem2.len);
    OledFunctions_stringToOledString("select me", menuItem2.ptr);
    menu->totalElements++;

    OledFunctions_optionSet(menu,0,NULL,&menuItem1);
    OledFunctions_optionSet(menu,1,first_stage_select,&menuItem2);

    OledFunctions_setGlobalMenu(menu);

}
