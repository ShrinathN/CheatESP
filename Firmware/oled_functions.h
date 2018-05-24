/*NOTE
 * This header file depends on the "i2c.h" file to function
 * The i2c.h file contains the implementation of soft i2c
 *
 * All the functions in this header will be generating the I2C starting condition, checking for ACK, and generating stop conditions
 * by themselves. This means the user does not have to generate a start condition separately, only the I2C init has to be done.
*/

//Comment out or remove these lines when compiling
//#include "user_config.h"
//#include "i2c.h"
#include "fonts.h"
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

uint8 characterCounter = 0;
#define NEWLINE_CHARACTER_THRESHOLD 24 //use this macros to define the threshold for new line, -1 it because the counter begins at 0

/* This function will do the following-
 * 1. Generate an I2C start condition
 * 2. Send the I2C slave address
 * 3. Check for ACK, if ACK continue, if NACK, return OLED_FAILURE
 * 4. Send the init string, if NACK, return OLED_FAILURE
 * 5.
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
	return OLED_SUCCESS; //return success
    }
    else
	return OLED_FAILURE; //return failure
}

/* This function will draw a character on the screen
 * and auto newline if the line threshold is reached
 * THIS FUNCTION DOES NOT GENERATE A START CONDITION, SEND I2C SLAVE ADDRESS OR GENERATE STOP CONDITION
 * for speed considerations, this function is meant to be used in the Oled_writeString function,
 * which will generate all of those for it
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
    return OLED_SUCCESS; //returns success
}

/*this function will check if the current line has been filled
 * basically divides the characterCounter with the NEWLINE_CHARACTER_THRESHOLD,
 * if its 0 (meaning the line is full), it sends 3 empty columns to fill the line
*/
void ICACHE_FLASH_ATTR
Oled_EndOfLine()
{
    if(!(characterCounter % NEWLINE_CHARACTER_THRESHOLD))
    {
	i2c_writeData(0x00);//3 columns of empty columns
	i2c_checkForAck();
	i2c_writeData(0x00);
	i2c_checkForAck();
	i2c_writeData(0x00);
	i2c_checkForAck();
    }
}

/*this function takes the cursor to the next line
 * first it prints empty characters while the line threshold is reached
 * after the threshold is reached, it fills the remaining 3 columns
*/
void ICACHE_FLASH_ATTR
Oled_newline()
{
    while(!(characterCounter % NEWLINE_CHARACTER_THRESHOLD))//loops while the line is not full
    {
	Oled_drawCharacter(fontCharacterArray[42]);//prints empty charcter while the line is not full
	characterCounter++;//increments character counter
    }
    Oled_EndOfLine();//to fill the 3 columns at the end of every line
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
    do
    {
	if(*array != 43) //43 is newline character
	    Oled_drawCharacter(fontCharacterArray[*(array)]);
	else if(*array == 43)//43 is the signal for a newline
	    Oled_newline();
	array++;
    }
    while(--length);
}

uint8 ICACHE_FLASH_ATTR
Oled_drawImage()
{

}

