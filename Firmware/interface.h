/* This file will deal with everything involving the physical part of the user interface
aka the button etc
*/

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

//comment when compiling
//#include "user_config.h"
//#include "oled_functions.h"

/* Test function to explore using a similar mechanism to drive the user interface
 *
*/

//this string should show, "whats up doc"
static uint8 docString[] = {0x16, 0x7, 0x0, 0x13, 0x12, 0x2a, 0x14, 0xf, 0x2a, 0x3, 0xe, 0x2};

//this string should show, "rabbit wabbit"
static uint8 rabbitString[] = {0x11, 0x0, 0x1, 0x1, 0x8, 0x13, 0x2a, 0x16, 0x0, 0x1, 0x1, 0x8, 0x13};


void ICACHE_FLASH_ATTR
buttonPressHandler(uint8 numberOfButtonPresses)
{
    if(numberOfButtonPresses == 1)
    {
        Oled_eraseScreen();
        Oled_returnCursor();
        Oled_writeString(docString, os_strlen(docString));
    }
    else if(numberOfButtonPresses == 2)
    {
        Oled_eraseScreen();
        Oled_returnCursor();
        Oled_writeString(rabbitString, os_strlen(rabbitString));
    }
}
