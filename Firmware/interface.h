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

enum stat
{
    TIME_MODE = 0x0,
    TEXT_MODE
};

struct _operationStatus_
{
    enum stat status;
}operationStatus;

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
    if(numberOfButtonPresses == 0)//this is supposed to be the time function
    {
        operationStatus.status = TIME_MODE;
        Oled_eraseScreen(); //first we erase the screen
        Oled_returnCursor(); //then we return the cursor to the start
        uint8 * timeSize = (uint8 *)os_zalloc(1); //allocates space for a temp variable to save string space
        *timeSize = os_strlen(sntp_get_real_time(realTime)); //length of time string
        OledString * tempTextBuffer = (OledString *)os_zalloc(*timeSize);// then we allocate x number of bytes to a tempTextBuffer
        stringToOledString(sntp_get_real_time(realTime), tempTextBuffer); //then we convert the realTime string to OledString, and store it in
        Oled_writeString(tempTextBuffer, *timeSize); //write string to the OLED
        os_free(tempTextBuffer); //free text buffer space
        os_free(timeSize); //frees up the string length variable
    }
    else if(numberOfButtonPresses == 1)//normal UI
    {
        if(operationStatus.status != TEXT_MODE) //transition to text mode
        {
            fileCounter = 0; //reseting  file counter
            operationStatus.status = TEXT_MODE; //setting the status as TEXT_MODE
        }
        os_sprintf(buffer, "%d", fileCounter); //prepare the name of the file to open
        espconn_send(esp, buffer, os_strlen(buffer)); //send the name of the file
        fileCounter++; //incement file counter
    }
    os_timer_disarm(&screenTimeoutTimer); //disarm screen timeout timer
    os_timer_setfn(&screenTimeoutTimer, screenTimeoutFunction, NULL); //set screen timeout timer function
    os_timer_arm(&screenTimeoutTimer, 5000, 0); //arm screen timout timer
}
