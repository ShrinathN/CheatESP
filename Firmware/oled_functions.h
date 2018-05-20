/*NOTE
 * This header file depends on the "i2c.h" file to function
 * The i2c.h file contains the implementation of soft i2c
 *
 * All the functions in this header will be generating the I2C starting condition, checking for ACK, and generating stop conditions
 * by themselves. This means the user does not have to generate a start condition separately, only the I2C init has to be done.
*/
#include "user_config.h"
#include "i2c.h"
#include "oled.h"
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
#define NEWLINE_CHARACTER_THRESHOLD 25
uint8 characterCounter = 0;

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
        while(localTempCounter < INIT_STRING_LENGTH) //while all bytes are note sent over, loop
        {
            i2c_writeData(OledInitString[localTempCounter]); //send the byte at which the counter is pointing to
            if(!i2c_checkForAck()) //if no ACK
                return OLED_FAILURE; //return failure
            localTempCounter++; //incrememt the counter
        }
        return OLED_SUCCESS; //return success
    }
    else
        return OLED_FAILURE; //return failure
}

/* This function will draw a character on the screen
 * and auto newline if the threshold is reached
 * THIS FUNCTION DOES NOT GENERATE A START CONDITION, SEND I2C SLAVE ADDRESS OR GENERATE STOP CONDITION
 * for speed considerations, this function is meant to be used in the Oled_writeString function,
 * which will generate all of those for it
*/
uint8 ICACHE_FLASH_ATTR
Oled_drawCharacter(uint8 * character)
{
    uint8 localTempColumn = 0; //using a temporary variable
    uint8 localTempCounter = 0;
    while(column < 4) //since all characters are 4 bytes
    {
        i2c_writeData(*(charater + localTempColumn)); //increments the base pointer of the character by localTempColumn counter
        if(!i2c_checkForAck()) //checks for ACK, if not returns error
            return OLED_FAILURE; //returns failure
        characterCounter++; //increments the counter indicating the number of characters on the screen
        if(characterCounter % NEWLINE_CHARACTER_THRESHOLD == 0) //if the line is full, go to next line
        {
            localTempCounter = 0; //to be used to go to the next line
            while(localTempCounter < 3) //loop to print 3 empty columns in the line and go to the next line
            {
                i2c_writeData(0x00); //blank
                if(!i2c_checkForAck()) //check for failure
                    return OLED_FAILURE;
                localTempCounter++; //incrementing newline character counter
            }
        }
        localTempColumn++; //increments localTempColumn counter
    }
    return OLED_SUCCESS; //returns success
}

//function to write a string on the screen
uint8 ICACHE_FLASH_ATTR
Oled_writeString(uint8)
{
}

uint8 ICACHE_FLASH_ATTR
Oled_drawImage()
{

}

