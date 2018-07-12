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

extern struct espconn * esp;

LOCAL os_timer_t screenTimeoutTimer;

uint8 fileCounter;
uint8 buffer[5];

/* Function to be executed when the screen has been idle for 5 seconds
*/
void ICACHE_FLASH_ATTR
screenTimeoutFunction()
{
    Oled_eraseScreen(); //first we erase the screen
    Oled_returnCursor(); //then we return the cursor to the start
}

/* This function will handle the changing of UI elements when the interface button is pressed a number of times
*/

uint8 tempVar;
void ICACHE_FLASH_ATTR
buttonPressHandler(uint8 numberOfButtonPresses)
{
    os_timer_disarm(&screenTimeoutTimer); //clears the currently running screen timeout timer

    tempVar = 26 + numberOfButtonPresses;
    Oled_writeString(&tempVar, 1);

    os_timer_disarm(&screenTimeoutTimer); //disarm screen timeout timer
    os_timer_setfn(&screenTimeoutTimer, screenTimeoutFunction, NULL); //set screen timeout timer function
    os_timer_arm(&screenTimeoutTimer, 5000, 0); //arm screen timout timer
}
