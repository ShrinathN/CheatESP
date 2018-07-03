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

/* This function will handle the changing of UI elements when the interface button is pressed a number of times
*/
void ICACHE_FLASH_ATTR
buttonPressHandler(uint8 numberOfButtonPresses)
{
    if(numberOfButtonPresses == 0)//this is supposed to be the time function
    {
        Oled_eraseScreen(); //first we erase the screen
        Oled_returnCursor(); //then we return the cursor to the start
        OledString * tempTextBuffer = (OledString *)os_zalloc(os_strlen(sntp_get_real_time(realTime)));// then we allocate x number of bytes to a tempTextBuffer
        stringToOledString(sntp_get_real_time(realTime), tempTextBuffer); //then we convert the realTime string to OledString, and store it in
        Oled_writeString(tempTextBuffer, sizeof(tempTextBuffer));
        os_free(tempTextBuffer);
    }
    else if(numberOfButtonPresses == 1)//normal UI
    {
        Oled_eraseScreen();
        Oled_returnCursor();
        Oled_writeString(rabbitString, sizeof(rabbitString));
    }
}
