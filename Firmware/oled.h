//general macro section
#define OFF 0
#define ON 1

//control bytes, to be sent after the address
//command control byte instructs that the following bytes will be command bytes
#define CONTROL_BYTE_COMMAND (0x00)

//similarly, data control byte instructs that the following bytes will be data bytes
#define CONTROL_BYTE_DATA (0x40)

//command to set contrast of the screen
//should be followed by a value 0-255
//default value is 0x7F
#define CMD_SET_CONTRAST (0x81)
#define DEFAULT_CONTRAST (0x7F)

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

//TO BE USED WITH PAGE ADDRESSING MODE
//set lower nibble of column start register
//default is 0x0
#define CMD_SET_COLUMN_START_NIBBLE_LOW(x) (0x00 | x)
#define DEFAULT_COLUMN_START_NIBBLE_LOW (0x0)

//TO BE USED WITH PAGE ADDRESSING MODE
//set high nibble of column start register
//default is 0x0
#define CMD_SET_COLUMN_START_NIBBLE_HIGH(x) (0x10 | x)
#define DEFAULT_COLUMN_START_NIBBLE_HIGH (0x0)

//set the memory addressing mode
//should be followed by a byte
//if 00b follows, horizontal addressing mode
//if 01b follows, vertical addressing mode
//if 10b follows, page addressing mode
//if 11b follows, invalid lol
#define CMD_SET_MEMORY_ADDRESSING_MODE (0x20)
#define MEMORY_ADDRESSING_MODE_HORIZONTAL (0x00)
#define MEMORY_ADDRESSING_MODE_VERTICAL (0x01)
#define MEMORY_ADDRESSING_MODE_PAGE (0x10)

//set the column start and end address
//must be followed by 2 bytes, both must be between 0 to 0x7F
//first byte is column start address
//second byte is column end address
#define CMD_SET_COLUMN_ADDRESS_RANGE (0x21)
#define SET_COLUMN_START_ADDRESS(x) (x & 0x80) //masking the MSB
#define SET_COLUMN_END_ADDRESS(x) (x & 0x80)
#define DEFAULT_COLUMN_START_ADDRESS (0x0)
#define DEFAULT_COLUMN_END_ADDRESS (0x7F)


//set the page start and end address
//must be followed by 2 bytes, both must be between 0 to 7
//this command is only for horizontal or vertical addressing mode
#define CMD_SET_PAGE_ADDRESS_RANGE (0x22)
#define SET_PAGE_START_ADDRESS(x) (x & 0x7)
#define SET_PAGE_END_ADDRESS(x) (x & 0x7)
#define DEFAULT_PAGE_START_ADDRESS (0x00)
#define DEFAULT_PAGE_END_ADDRESS (0x07)

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

//command to set the multiplex ratio
//should be followed by a byte, ranging from 0 to 0x3F
//default is 0x3Fh
#define CMD_SET_MUX_RATIO (0xA8)
#define DEFAULT_MUX_RATIO (0x3F)

//NOTE: i have no idea what this does and my screen died before i could test it out
//command to set the vertical shift, display offset
//should be followed by a byte from 0 to 0x63
//default is 0
#define CMD_SET_DISPLAY_OFFSET (0xD3)
#define DEFAULT_DISPLAY_OFFSET (0x00)

//command to set com output scan direction
//x can be either 0 or 1, making the command 0xC0 or 0xC8
#define CMD_SET_COM_OUTPUT_SCAN(x) (0xC0 | (x << 3))

//command to set com pin hardware configuration
//should be followed by a byte
//x can be either 0 or 1, y can be either 0 or 1
//0 0 y x 0 0 1 0
//if x = 0, sequential pin configuration will be followed
//if x = 1, alternate pin configuration will be followed (default)
//if y = 0, disable com left/right remap (default)
//if y = 1, enable com left/right remap
#define CMD_SET_COM_PIN_CONFIG (0xDA)
#define SET_COM_PIN_CONFIG(x,y) (0x2 | (x << 4) | (y << 5))
#define DEFAULT_COM_PIN_CONFIG (0x12)

//command to configure the charge pump
//if x = 0, the charge pump is disabled (default)
//if x = 1, the charge pump is enabled when the display is on
#define CMD_TOGGLE_CHARGE_PUMP (0x8D)
#define SET_CHARGE_PUMP_STATUS(x) (0x10 | (x << 2))


#define INIT_STRING_LENGTH 20
char OledinitString[INIT_STRING_LENGTH] = {
    CONTROL_BYTE_COMMAND,//control byte, following transmission will contain command data bytes
    CMD_SET_DISPLAY_ON(OFF),//display off

    CMD_SET_MUX_RATIO,//setting mux ratio
    DEFAULT_MUX_RATIO,//64mux, reset 0b00111111

    CMD_SET_DISPLAY_OFFSET,//set vertical shift by com
    DEFAULT_DISPLAY_OFFSET,//00h, reset

    CMD_SET_DISPLAY_START_LINE(0x00),//set display start line as 00, reset
    CMD_SET_SEGMENT_REMAP(ON),//segment remap, column address 127 mapped to SEG0
    CMD_SET_COM_OUTPUT_SCAN(ON),//remapped mode, scan COM ports in reverse

    CMD_SET_COM_PIN_CONFIG,//set COM pin hardware config
    DEFAULT_COM_PIN_CONFIG,//alternative COM pin config

    CMD_SET_CONTRAST,//set contrast
    DEFAULT_CONTRAST,//reset

    CMD_ENTIRE_DISPLAY_ON(OFF),//resume to ram content display, reset
    CMD_SET_DISPLAY_INVERTED(OFF),//normal display, reset

    CMD_SET_DISPLAY_CLOCK,//set display clock
    0xf0,//not reset

    CMD_TOGGLE_CHARGE_PUMP,//charge pump
    SET_CHARGE_PUMP_STATUS(ON),//enable charge pump during display on

    CMD_SET_DISPLAY_ON(ON)//turn on display
};
//=====starting of full range string=====
#define SET_FULL_RANGE_STRING_LENGTH 11
unsigned char OledsetFullRangeString[SET_FULL_RANGE_STRING_LENGTH] = {
    CONTROL_BYTE_COMMAND,//control byte, following transmission will contain command data bytes

    CMD_SET_COLUMN_START_NIBBLE_LOW(DEFAULT_COLUMN_START_NIBBLE_LOW),
    CMD_SET_COLUMN_START_NIBBLE_HIGH(DEFAULT_COLUMN_START_NIBBLE_HIGH),

    CMD_SET_COLUMN_ADDRESS_RANGE,//set column start and end address
    DEFAULT_COLUMN_START_ADDRESS,//start address, reset
    DEFAULT_COLUMN_END_ADDRESS,//end address, return

    CMD_SET_PAGE_ADDRESS_RANGE,//set page start and end address
    DEFAULT_PAGE_START_ADDRESS,//start address, reset
    DEFAULT_PAGE_END_ADDRESS,//end address reset

    CMD_SET_MEMORY_ADDRESSING_MODE,
    MEMORY_ADDRESSING_MODE_HORIZONTAL
};
