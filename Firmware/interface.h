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

extern struct espconn * esp; //for comm
extern uint32 realTime; //real time


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

void ICACHE_FLASH_ATTR
buttonPressHandler(uint8 numberOfButtonPresses)
{
    os_timer_disarm(&screenTimeoutTimer); //clears the currently running screen timeout timer
    if(numberOfButtonPresses == 0) //this is supposed to show the time
    {
        uint8 timeBufferLength = strlen(sntp_get_real_time(realTime)); //getting the real time string length
        uint8 * timeBuffer = (uint8 * )os_zalloc(timeBufferLength); //allocating memory for the temp buffer to store the real time
        OledStringPtr * oledTimeString = (OledStringPtr *)os_zalloc(timeBufferLength); //allocating memory for oledstring containing the time
        os_sprintf(timeBuffer, "%s", sntp_get_real_time(realTime)); //prints copies the real time string to
        Oled_stringToOledString(timeBuffer, oledTimeString); //converts the string to oled string
        OledStringStruct * oledTimeStringStruct = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
        oledTimeStringStruct->len = timeBufferLength;
        oledTimeStringStruct->ptr = oledTimeString;

        Oled_eraseScreen(); //clearing screen
        Oled_returnCursor(); //returning cursor
        Oled_writeString(oledTimeStringStruct); //writing the time string

        os_free(timeBuffer); //deallocating memory
        os_free(oledTimeString);
        os_free(oledTimeStringStruct);
    }
    else if(numberOfButtonPresses == 1)
    {

    }

    os_timer_disarm(&screenTimeoutTimer); //disarm screen timeout timer
    os_timer_setfn(&screenTimeoutTimer, screenTimeoutFunction, NULL); //set screen timeout timer function
    os_timer_arm(&screenTimeoutTimer, 5000, 0); //arm screen timout timer
}
