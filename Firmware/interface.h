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

#define SHORT_PRESS 0
#define LONG_PRESS 1

//comment when compiling
//#include "user_config.h"
//#include "oled_functions.h"

extern MenuStruct GlobalMenuStruct;
extern struct espconn * esp; //for comm
extern uint32 realTime; //real time

LOCAL os_timer_t screenTimeoutTimer;

/* This function will be executed when there's a short press
 * It will first check if the screen is on
 * If the screen is off, it will turn on the screen for 5 seconds
 * If the screen is on, it will
*/
void ICACHE_FLASH_ATTR
next()
{
    if(screenStatus.screenOn == 0) //if the screen is off
    {
        Oled_printMenuInfo(&GlobalMenuStruct);
        Oled_setScreenOn(1); //turn it on
    }
    else //meaning the screen is already on
    {
        GlobalMenuStruct.currentElement++; //increment selected element counter
        Oled_drawGlobalMenu();
    }
}

void ICACHE_FLASH_ATTR
select()
{
    if(screenStatus.screenOn == 0) //the screen is off
        Oled_setScreenOn(1);
    else //the screen is already on
    {
        void (*toRun)();
        toRun = (void *)GlobalMenuStruct.menuFunctions[GlobalMenuStruct.currentElement];
        toRun();
    }
}

/* Function to be executed when the screen has been idle for 5 seconds
*/
void ICACHE_FLASH_ATTR
screenTimeoutFunction()
{
    Oled_setScreenOn(0);
}

/* This function will handle the button press
*/

void ICACHE_FLASH_ATTR
buttonPressHandler(uint8 numberOfButtonPresses)
{
    os_timer_disarm(&screenTimeoutTimer); //clears the currently running screen timeout timer

    if(numberOfButtonPresses == SHORT_PRESS) //this is supposed to show the time
        next();
    else if(numberOfButtonPresses == LONG_PRESS)
        select();

    os_timer_disarm(&screenTimeoutTimer); //disarm screen timeout timer
    os_timer_setfn(&screenTimeoutTimer, screenTimeoutFunction, NULL); //set screen timeout timer function
    os_timer_arm(&screenTimeoutTimer, SCREEN_TIMEOUT_MS, 0); //arm screen timout timer
}
