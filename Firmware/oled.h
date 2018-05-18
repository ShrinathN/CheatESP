//general macro section
#define OFF 0
#define ON 1
//command to set contrast of the screen
//should be followed by a value 0-255
#define CMD_SET_CONTRAST (0x81)

//display on settings
//if x = 0, resume to RAM content display, output follows RAM content
//if x = 1, entire dispay on, output ignores RAM content
//note - might be useful for hardware debugging
#define CMD_ENTIRE_DISPLAY_ON(x) (0xa4 | x)

//command to invert the screen
//if x = 0, 0 in RAM = off, 1 in RAM = on
//if x = 1, 1 in RAM = off, 0 in RAM = on
#define CMD_SET_DISPLAY_INVERTED(x) (0xa6 | x)

//turn display on or off
//if x = 0, display off, sleep mode
//if x = 1, display on
#define CMD_SET_DISPLAY_ON(x) (0xae | x)

//deactivate scroll
#define DEACTIVATE_SCREEN_SCROLL 0x2e

//set lower nibble of column start register
//might want to use 0x0
#define CMD_SET_COLUMN_START_NIBBLE_LOW(x) (0x00 | x)

//set high nibble of column start register
//might want to use 0x0
#define CMD_SET_COLUMN_START_NIBBLE_HIGH(x) (0x10 | x)

//set the memory addressing mode
//* * * * * * X X
//7 6 5 4 3 2 1 0
//if 00b follows, horizontal addressing mode
//if 01b follows, vertical addressing mode
//if 10b follows, page addressing mode
//if 11b follows, invalid lol
#define CMD_SET_MEMORY_ADDRESSING_MODE (0x20)

//set the column start and end address
//must be followed by 2 bytes, both must be between 0d-127d
#define CMD_SET_COLUMN_ADDRESS_RANGE (0x21)

//set the page start and end address
//must be followed by 2 bytes, both must be between 0-7d
#define CMD_SET_PAGE_ADDRESS_RANGE (0x22)

//set the page start address
//can choose from page 0 to page 7
//x can be from 0d to 7d
#define CMD_SET_PAGE_START_ADDRESS(x) (0xb0 | x)

//set display start line register
//the line can be from 0d to 63d
#define CMD_SET_DISPLAY_START_LINE(x) (0x40 | x)

//set segment re-map
//if x = 0, column address 0 is mapped to SEG0
//if x = 1, column address 127 is mapped to SEG0
#define CMD_SET_SEGMENT_REMAP(x) (0xa0 | x)

//set display clock and divide ratio
//must be followed by a byte
//upper nibble of byte is F_osc, increases with value
//lower nibble is divide ration (0000b is 1d)
#define CMD_SET_DISPLAY_CLOCK (0xd5)

//control bytes, to be sent after the address
//command control byte instructs the following bytes will be command bytes
#define CONTROL_BYTE_COMMAND (0x00)
//similarly, data control byte instructs the following bytes will be data bytes
#define CONTROL_BYTE_DATA (0x40)

//command to set the multiplex ratio
//should be followed by a byte, ranging from 0 to 0x3Fh
#define CMD_SET_MULTIPLEX_RATIO (0xA8)

