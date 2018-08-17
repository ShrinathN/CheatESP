#ifndef OLED_FUNCTIONS_H
#define OLED_FUNCTIONS_H

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

#define OLED_ADDRESS 0x78 //address of the I2C OLED device, change if needed
#define OLED_SUCCESS 0 //macro to indicate success
#define OLED_FAILURE 1 //macro to indicate failure

/* This variable holds the number of characters that have been drawn on the screen, basically it acts as a counter
 * in a normal 128x64 OLED, considering 8x4 fonts, and one column as space between every character,
 * a total of 25 characters can be displayed on the screen (since 128 / 5 =  25.6 ~ 25)
 * Since the screen is 64 pixels high, and each row of characters takes 8 pixels, this equals to (64 / 8 = 8) 8 rows
 * Hence we get a total character count of (25 * 8 = 200) 200 characters
 * Be sure to change the threshold if a different screen is being used
*/

static uint8 characterCounter = 0; //counter to keep a check of the number of characters on the screen right now
#define NEWLINE_CHARACTER_THRESHOLD 25 //use this macros to define the threshold for new line
#define SCREEN_THRESHOLD 200
#define NEWLINE_CHARACTER 70 //newline

typedef uint8 OledStringPtr;
typedef uint8 OledStringLen;
typedef void (*MenuCallbackFunction)(void);

// this struct will store the info about the screen
struct _screenStatus_
{
    uint8 screenOn : 1; // is the screen on? 1 bit, 1 for true, 0 for false
}screenStatus;

// This struct will define a a OledStringStruct
// It is meant to be used with only an oled font mapped strings
typedef struct
{
    OledStringPtr * ptr; //this is a pointer to the oled string
    OledStringLen len; //length of the OLED string
}OledStringStruct;

// this struct defines the MenuStruct which contains all the details about drawing a menu
// the callback function will have to be registered for each menu item
typedef struct
{
    OledStringStruct *menuStrings[8]; //this will store the text related to all the menu elements
    MenuCallbackFunction *menuFunctions[8]; //this array will store the callback functions to
    uint8 currentElement : 3; //this is the current element
    uint8 totalElements : 3; //this is the number of total elements
}MenuStruct;

// this is the pointer to a menustruct which will be used to create the menu
MenuStruct GlobalMenuStruct;

//function definitions
uint8 OledFunctions_init(void);
void OledFunctions_EndOfLine(void);
uint8 OledFunctions_drawCharacter(uint8 *);
void OledFunctions_newline(void);
void OledFunctions_commStart(uint8);
void OledFunctions_commStop(void);
uint8 OledFunctions_writeString(OledStringStruct *oledString);
uint8 OledFunctions_eraseScreen(void);
void OledFunctions_returnCursor(void);
void OledFunctions_setScreenOn(uint8);
uint8 * OledFunctions_stringToOledString(char * string, OledStringPtr * buffer);
void OledFunctions_optionSet(MenuStruct *menu, uint8 itemNumber, MenuCallbackFunction callback, OledStringStruct *oledStringStruct);
void OledFunctions_setGlobalMenu(MenuStruct *);
void OledFunctions_drawMenu(MenuStruct *);
void OledFunctions_drawGlobalMenu(void);
void OledFunctions_printMenuInfo(MenuStruct *);


#endif
