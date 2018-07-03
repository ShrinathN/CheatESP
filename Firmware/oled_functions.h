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

//Comment out or remove these lines when compiling
#include "user_config.h"
//#include "i2c.h"
//#include "fonts.h"
//#include "oled.h"
#define OLED_ADDRESS 0x78 //address of the I2C OLED device, change if needed
#define OLED_SUCCESS 0 //macro to indicate success
#define OLED_FAILURE -1 //macro to indicate failure

/* This variable holds the number of characters that have been drawn on the screen, basically it acts as a counter
 * in a normal 128x64 OLED, considering 8x4 fonts, and one column as space between every character,
 * a total of 25 characters can be displayed on the screen (since 128 / 5 =  25.6 ~ 25)
 * Since the screen is 64 pixels high, and each row of characters takes 8 pixels, this equals to (64 / 8 = 8) 8 rows
 * Hence we get a total character count of (25 * 8 = 200) 200 characters
 * Be sure to change the threshold if a different screen is being used
*/

uint8 characterCounter = 0; //counter to keep a check of the number of characters on the screen right now
#define NEWLINE_CHARACTER_THRESHOLD 25 //use this macros to define the threshold for new line
#define SCREEN_THRESHOLD 200
#define NEWLINE_CHARACTER 70 //newline

typedef uint8 OledString;

/* This function will do the following-
 * 1. Generate an I2C start condition
 * 2. Send the I2C slave address
 * 3. Send the init string
 * 4. Send the normal full range useage string
 * 5. Generate I2C stop condition
 *
 * Will return an OLED_FAILURE if NACK
*/
uint8 ICACHE_FLASH_ATTR
Oled_init()
{
    uint8 localTempCounter = 0; //temporary counter
    i2c_startCondition(); //start conditon for comm initialization
    i2c_writeData(OLED_ADDRESS); //sending over the I2C slave address
    if(i2c_checkForAck()) //checks for ACK
    {
        do
        {
            i2c_writeData(OledinitString[localTempCounter]); //send the byte at which the counter is pointing to
            if(!i2c_checkForAck()) //if no ACK
                return OLED_FAILURE; //return failure
        }
        while(localTempCounter++ < INIT_STRING_LENGTH); //while all bytes are note sent over, loop
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
Oled_EndOfLine()
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
Oled_drawCharacter(uint8 * character)
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
        Oled_EndOfLine(); //to fill the 3 columns left
    if(characterCounter >= SCREEN_THRESHOLD) //reset the character counter if the screen is full
        characterCounter = 0;
    return OLED_SUCCESS; //returns success
}

/*this function takes the cursor to the next line
 * first it prints empty characters while the line threshold is reached
 * after the threshold is reached, it calls Oled_EndOfLine function to fill the remaining 3 columns
*/
void ICACHE_FLASH_ATTR
Oled_newline()
{
    do
    {
        Oled_drawCharacter(fontCharacterArray[42]);//prints empty charcter while the line is not full, don't need to increment characterCounter
    }
    while((characterCounter % NEWLINE_CHARACTER_THRESHOLD) > 0);//loops while the line is not full
}

void ICACHE_FLASH_ATTR
Oled_commStart()
{
    i2c_startCondition();
    i2c_writeData(OLED_ADDRESS);
    i2c_checkForAck();
    i2c_writeData(CONTROL_BYTE_DATA);
    i2c_checkForAck();
}

void ICACHE_FLASH_ATTR
Oled_commStop()
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
Oled_writeString(uint8 * array, uint8 length)
{
    Oled_commStart();
    do
    {
        if(*array != NEWLINE_CHARACTER)
        {
            Oled_drawCharacter(fontCharacterArray[*(array)]);
        }
        else
        {
            Oled_newline(); //newline if NEWLINE_CHARACTER is encountered
        }
        array++;
    }
    while(--length);
    Oled_commStop();
}

/*function to erase the screen
 * resets the character counter back to 0, clears screen, generates start condition and stop condition
*/
uint8 ICACHE_FLASH_ATTR
Oled_eraseScreen()
{
    Oled_commStart();
    uint16 erasecounter; //counter to erase the screen
    for(erasecounter = 0; erasecounter < 128*8; erasecounter++) //loop while the whole screen hasn't been processed
    {
        i2c_writeData(0x0);//write a 0x0 to clear the column
        if(!i2c_checkForAck())//check for ACK
            return OLED_FAILURE;
    }
    Oled_commStop();
    return OLED_SUCCESS;
}

void ICACHE_FLASH_ATTR
Oled_returnCursor()
{
    Oled_commStart();
#ifdef DEBUG_ENABLE
    os_printf("Character Counter = %d\n",characterCounter);
#endif
    while(characterCounter != 0)
    {
        Oled_drawCharacter(fontCharacterArray[42]);
    }
    Oled_commStop();
}

uint8 * ICACHE_FLASH_ATTR
stringToOledString(char * string, OledString * buffer)
{
    uint8 counter, byteRead, toDisplay;
    for(counter = 0; counter < os_strlen(string); counter++)
    {
        byteRead = string[counter];
        if(byteRead > 96 && byteRead < 123) //lower case
        {
            toDisplay = byteRead - 97;
        }
        else if(byteRead > 64 && byteRead < 91) //upper case
        {
            toDisplay = byteRead - 65;
        }
        else if(byteRead > 47 && byteRead < 58) //number
        {
            toDisplay = (byteRead - 47) + 25;
        }
        else if(byteRead == 32) //space character
        {
            toDisplay = 42;
        }
        else if(byteRead == '\n') //newline character
        {
            toDisplay = 70;
        }
        else if(byteRead == '.') //.
        {
            toDisplay = 36;
        }
        else if(byteRead == ',') //,
        {
            toDisplay = 37;
        }
        else if(byteRead == '+') //+
        {
            toDisplay = 38;
        }
        else if(byteRead == '-') //-
        {
            toDisplay = 39;
        }
        else if(byteRead == '*') //.
        {
            toDisplay = 40;
        }
        else if(byteRead == '/') //.
        {
            toDisplay = 41;
        }
        else if(byteRead == ':') //.
        {
            toDisplay = 43;
        }
        buffer[counter] = toDisplay;
    }
}
