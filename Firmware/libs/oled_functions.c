// This file acts as a wrapper between the I2C function calls and the OLED command calls
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
#include "oled_functions.h"
#include "i2c.h"
#include "fonts.h"
#include "oled.h"


/* This function will do the following-
 * 1. Generate an I2C start condition
 * 2. Send the I2C slave address
 * 3. Send the init string
 * 4. Send the normal full range useage command string
 * 5. Generate I2C stop condition
 *
 * Will return an OLED_FAILURE if NACK
*/
uint8 ICACHE_FLASH_ATTR
OledFunctions_init()
{
    uint8 localTempCounter = 0; //temporary counter
    i2c_startCondition(); //start conditon for comm initialization
    i2c_writeData(OLED_ADDRESS); //sending over the I2C slave address
    if(i2c_checkForAck()) //checks for ACK
    {
        do
        {
            i2c_writeData((uint8)OledinitString[localTempCounter]); //send the byte at which the counter is pointing to
            if(!i2c_checkForAck()) //if no ACK
                return OLED_FAILURE; //return failure
        }
        while(localTempCounter++ < INIT_STRING_LENGTH); //while all bytes are not sent over, loop
        i2c_stopCondition();

        localTempCounter = 0; //reseting
        i2c_startCondition(); //start conditon for comm initialization
        i2c_writeData(OLED_ADDRESS); //sending over the I2C slave address
        if(i2c_checkForAck()) //checks for ACK
        {
            do
            {
                i2c_writeData(OledsetFullRangeString[localTempCounter]); //send the byte at which the counter is pointing to
                if(!i2c_checkForAck()) //if no ACK
                    return OLED_FAILURE; //return failure
            }
            while(localTempCounter++ < SET_FULL_RANGE_STRING_LENGTH); //while all bytes are note sent over, loop
            i2c_stopCondition(); //stop condition, init is done now
            return OLED_SUCCESS;
        }
        else
            return OLED_FAILURE; //return failure
    }
    else
        return OLED_FAILURE; //return failure
}

/*this function will check if the current line has been filled
 * basically divides the characterCounter with the NEWLINE_CHARACTER_THRESHOLD,
 * if its 0 (meaning the line is full), it sends 3 empty columns to fill the line
*/
void ICACHE_FLASH_ATTR
OledFunctions_EndOfLine()
{
    if(!(characterCounter % NEWLINE_CHARACTER_THRESHOLD)) //redundant check if the line is full
    {
        i2c_writeData(0x00);//3 columns of empty columns
        i2c_checkForAck();
        i2c_writeData(0x00);
        i2c_checkForAck();
        i2c_writeData(0x00);
        i2c_checkForAck();
    }
}

/* This function will draw a character on the screen
 * and auto newline if the line threshold is reached
 * THIS FUNCTION DOES NOT GENERATE A START CONDITION, SEND I2C SLAVE ADDRESS OR GENERATE STOP CONDITION
 * this function is meant to be used in the Oled_writeString function,
 * which will generate all of those for it
 * Will also increment the characterCounter at every character draw
*/
uint8 ICACHE_FLASH_ATTR
OledFunctions_drawCharacter(uint8 * character)
{
    uint8 localTempColumn; //using a temporary variable
    for(localTempColumn = 0;localTempColumn < 4; localTempColumn++) //since all characters are 4 bytes
    {
        i2c_writeData(*(character + localTempColumn)); //increments the base pointer of the character by localTempColumn counter, and writes the data using I2C
        if(!i2c_checkForAck()) //checks for ACK, if not returns error
            return OLED_FAILURE; //returns failure
    }
    i2c_writeData(0x00);//empty column between two characters
    if(!i2c_checkForAck()) //checks for ACK, if not returns error
        return OLED_FAILURE; //returns failure
    characterCounter++; //increments the counter indicating the number of characters on the screen
    if(characterCounter % NEWLINE_CHARACTER_THRESHOLD == 0) //if the current line is full, newline
        OledFunctions_EndOfLine(); //to fill the 3 columns left
    if(characterCounter >= SCREEN_THRESHOLD) //reset the character counter if the screen is full
        characterCounter = 0;
    return OLED_SUCCESS; //returns success
}

/*this function takes the cursor to the next line
 * first it prints empty characters while the line threshold is reached
 * after the threshold is reached, it calls Oled_EndOfLine function to fill the remaining 3 columns
*/
void ICACHE_FLASH_ATTR
OledFunctions_newline()
{
    do
    {
        OledFunctions_drawCharacter(fontCharacterArray[42]);//prints empty charcter while the line is not full, don't need to increment characterCounter
    }
    while((characterCounter % NEWLINE_CHARACTER_THRESHOLD) > 0);//loops while the line is not full
}


/* This function will start communication with the OLED display
 * the data which is going to come next can be either display data (CONTROL_BYTE_DATA) or command data (CONTROL_BYTE_COMMAND)
*/
void ICACHE_FLASH_ATTR
OledFunctions_commStart(uint8 controlByteType)
{
    i2c_startCondition();
    i2c_writeData(OLED_ADDRESS);
    i2c_checkForAck();
    i2c_writeData(controlByteType);
    i2c_checkForAck();
}

/* This function will generate the i2c stop condition necessary to stop communications with
 * the OLED display
*/
void ICACHE_FLASH_ATTR
OledFunctions_commStop()
{
    i2c_stopCondition();
}


/* NOTE
 * This function is experimental, invoking this function MAY cause the watchdog to reset
 * If so, this will be replaced by a os_task or os_timer implementation
 *
 * This function will handle the writing of strings on the screen
 * The "array" must contain the character to print according to the font map in fonts.h file
*/
uint8 ICACHE_FLASH_ATTR
OledFunctions_writeString(OledStringStruct *oledString)
{
    uint8 counter;
    OledStringPtr *ptr = oledString->ptr;
    OledStringLen length = oledString->len;
    OledFunctions_commStart(CONTROL_BYTE_DATA);
    do
    {
        if(*ptr != NEWLINE_CHARACTER)
        {
            OledFunctions_drawCharacter(fontCharacterArray[*ptr]);
        }
        else
        {
            OledFunctions_newline(); //newline if NEWLINE_CHARACTER is encountered
        }
        ptr++;
    }
    while(--length);
    OledFunctions_commStop();
}

/*function to erase the screen
 * resets the character counter back to 0, clears screen, generates start condition and stop condition
*/
uint8 ICACHE_FLASH_ATTR
OledFunctions_eraseScreen()
{
    OledFunctions_commStart(CONTROL_BYTE_DATA);
    uint16 erasecounter; //counter to erase the screen
    for(erasecounter = 0; erasecounter < 128*8; erasecounter++) //loop while the whole screen hasn't been processed
    {
        i2c_writeData(0x0);//write a 0x0 to clear the column
        if(!i2c_checkForAck())//check for ACK
            return OLED_FAILURE;
    }
    OledFunctions_commStop();
    return OLED_SUCCESS;
}

/* This function will return the cursor back at the starting position of the screen
*/
void ICACHE_FLASH_ATTR
OledFunctions_returnCursor()
{
    OledFunctions_commStart(CONTROL_BYTE_DATA);
    while(characterCounter != 0)
    {
        OledFunctions_drawCharacter(fontCharacterArray[42]);
    }
    OledFunctions_commStop();
}

/* this function is used to turn the OLED display on and off
*/
void ICACHE_FLASH_ATTR
OledFunctions_setScreenOn(uint8 setScreen)
{
    if(setScreen == 0 || setScreen == 1)
    {
        OledFunctions_commStart(CONTROL_BYTE_COMMAND);
        i2c_writeData(CMD_SET_DISPLAY_ON(setScreen));
        i2c_checkForAck();
        OledFunctions_commStop();
        screenStatus.screenOn = setScreen;
    }
}

/* This string will be used to convert a normal ASCII string to an OLED font
 * mapped string and stored at the buffer address
*/
uint8 * ICACHE_FLASH_ATTR
OledFunctions_stringToOledString(char * string, OledStringPtr * buffer)
{
    uint8 counter, byteRead, toDisplay;
    for(counter = 0; counter < os_strlen(string); counter++)
    {
        byteRead = (uint8)string[counter];
        if(byteRead > 96 && byteRead < 123) //lower case
            toDisplay = byteRead - 97;
        else if(byteRead > 64 && byteRead < 91) //upper case
            toDisplay = byteRead - 65;
        else if(byteRead > 47 && byteRead < 58) //number
            toDisplay = (byteRead - 47) + 25;
        else if(byteRead == 32) //space character
            toDisplay = 42;
        else if(byteRead == '\n') //newline character
            toDisplay = 70;
        else if(byteRead == '.') //.
            toDisplay = 36;
        else if(byteRead == ',') //,
            toDisplay = 37;
        else if(byteRead == '+') //+
            toDisplay = 38;
        else if(byteRead == '-') //-
            toDisplay = 39;
        else if(byteRead == '*') //.
            toDisplay = 40;
        else if(byteRead == '/') //.
            toDisplay = 41;
        else if(byteRead == ':') //.
            toDisplay = 43;
        buffer[counter] = toDisplay;
    }
}

/* This function will set the callback function and string for a given menu
*/
void ICACHE_FLASH_ATTR
OledFunctions_optionSet(MenuStruct *menu,uint8 itemNumber, MenuCallbackFunction callback, OledStringStruct *oledStringStruct)
{
    menu->menuStrings[itemNumber] = oledStringStruct;
    menu->menuFunctions[itemNumber] = (MenuCallbackFunction*)callback;
}

/* this function will set a given menu struct as the global menu struct
 * which will be used to draw the menu on an interrupt
*/
void ICACHE_FLASH_ATTR
OledFunctions_setGlobalMenu(MenuStruct *menu)
{
    uint8 tempCounter;
    GlobalMenuStruct.currentElement = menu->currentElement;
    GlobalMenuStruct.totalElements = menu->totalElements;
    for(tempCounter = 0; tempCounter < menu->totalElements; tempCounter++)
    {
        GlobalMenuStruct.menuFunctions[tempCounter] = menu->menuFunctions[tempCounter];
        GlobalMenuStruct.menuStrings[tempCounter] = menu->menuStrings[tempCounter];
    }
}

/* This function will be used to draw a menustruct.
*/
void ICACHE_FLASH_ATTR
OledFunctions_drawMenu(MenuStruct * menu)
{
    if(menu->currentElement >= menu->totalElements) //checking if the current menu element is greater than total elements
        menu->currentElement = 0; //if so, reset the current element counter
    OledFunctions_eraseScreen(); //erase the screen
    OledFunctions_returnCursor(); //return the cursor
    OledStringStruct * tempOledStringStruct = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct)); //allocate space for the string struct to display newline character
    OledStringPtr characterToDraw = NEWLINE_CHARACTER; //setting the character to NEWLINE_CHARACTER, will print after each menu entry
    tempOledStringStruct->len = 1; //setting the length of the temp string struct as 1
    tempOledStringStruct->ptr = &characterToDraw; //setting the character to draw
    uint8 tempCounter; //temp counter hehe
    for(tempCounter = 0; tempCounter < menu->totalElements; tempCounter++) //will loop over all menu items
    {
        OledFunctions_writeString((OledStringStruct *)menu->menuStrings[tempCounter]); //draw the menu item at tempCounter
        if(tempCounter == menu->currentElement) //highlight the current element
        {
            characterToDraw = 40; //star symbol
            OledFunctions_writeString(tempOledStringStruct); //draw the symbol
        }
        characterToDraw = NEWLINE_CHARACTER; //setting the character back to NEWLINE_CHARACTER
        OledFunctions_writeString(tempOledStringStruct); //draws the newline
    }
    os_free(tempOledStringStruct);
}

void ICACHE_FLASH_ATTR
OledFunctions_drawGlobalMenu()
{
    OledFunctions_drawMenu(&GlobalMenuStruct);
}

void ICACHE_FLASH_ATTR
OledFunctions_printMenuInfo(MenuStruct * menu)
{
    uint8 elementCounter, stringCounter;
    os_printf("menu : %x\n"
              "menu->menuStrings : %x\n"
              "menu->currentElement : %d\n"
              "menu->totalElements : %d\n",
              (unsigned int)menu, (unsigned int)menu->menuStrings, menu->currentElement,
              menu->totalElements);
    for(elementCounter = 0; elementCounter < menu->totalElements; elementCounter++)
    {
        os_printf("menu->menuFunctions[%d] : %x\n\n"
                  "menu->menuStrings[%d]->ptr : %x\n"
                  "menu->menuStrings[%d]->len : %d\n",
                  elementCounter,(unsigned int)menu->menuFunctions[elementCounter],
                  elementCounter,(unsigned int)menu->menuStrings[elementCounter]->ptr,
                  elementCounter,menu->menuStrings[elementCounter]->len);
        for(stringCounter = 0; stringCounter < menu->menuStrings[elementCounter]->len; stringCounter++)
        {
            os_printf("menu->menuStrings[%d]->ptr[%d] : %d\n",
                      elementCounter,stringCounter, menu->menuStrings[elementCounter]->ptr[stringCounter]);
        }
    }
}
