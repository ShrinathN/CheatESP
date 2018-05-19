#include "user_config.h"
#include "i2c.h"
#include "oled.h"
#include "fonts.h"
#define AP_SSID ""
#define AP_PASSWORD ""
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)
void ICACHE_FLASH_ATTR func();
void timfunc();
void erase_screen();
char initSequence[20] = {CONTROL_BYTE_COMMAND,//control byte, following transmission will contain command data bytes
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

                         CMD_ENTIRE_DISPLAY_ON(ON),//resume to ram content display, reset
                         CMD_SET_DISPLAY_INVERTED(OFF),//normal display, reset

                         CMD_SET_DISPLAY_CLOCK,//set display clock
                         0xf0,//not reset

                         CMD_TOGGLE_CHARGE_PUMP,//charge pump
                         SET_CHARGE_PUMP_STATUS(ON),//enable charge pump during display on

                         CMD_SET_DISPLAY_ON(ON),//turn on display
                        };
uint8 byteToWrite;
unsigned char setFullRange[11] = {CONTROL_BYTE_COMMAND,//control byte, following transmission will contain command data bytes

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
LOCAL os_timer_t tim;
void user_init(void)
{
    os_delay_us(500000);
    func();
}
uint16 count;
void ICACHE_FLASH_ATTR func()
{
    i2c_init();
    i2c_startCondition();
    i2c_writeData(0x78);
    i2c_checkForAck();
    for(count = 0; count < 20; count++)
    {
        i2c_writeData(initSequence[count]);
        if(i2c_checkForAck())
            os_printf("ACK\n");
        else
            os_printf("NACK\n");
    }
    i2c_stopCondition();

    i2c_startCondition();
    i2c_writeData(0x78);
    i2c_checkForAck();
    for(count = 0; count < 11; count++)
    {
        i2c_writeData(setFullRange[count]);
        if(i2c_checkForAck())
            os_printf("ACK\n");
        else
            os_printf("NACK\n");
    }
    i2c_stopCondition();

    i2c_startCondition();
    i2c_writeData(0x78);
    i2c_checkForAck();
    i2c_writeData(0x40);
    i2c_checkForAck();

    count = 0;
    os_timer_disarm(&tim);
    os_timer_setfn(&tim, erase_screen, NULL);
    os_timer_arm(&tim, 1,1);
    byteToWrite = 0xff;

}

void timfunc()
{
    uint8 localcount;
    if(count % 25 == 0)
    {
        i2c_writeData(0x0);
        i2c_checkForAck();
        i2c_writeData(0x0);
        i2c_checkForAck();
        i2c_writeData(0x0);
        i2c_checkForAck();
    }
    if(count > 41)
    {
        os_timer_disarm(&tim);
    }
    else
    {
        for(localcount = 0; localcount < 4; localcount++)
        {
            i2c_writeData(characters[count][localcount]);
            i2c_checkForAck();
        }
        i2c_writeData(0x0);
        i2c_checkForAck();
        count++;
    }
}

uint16 erasecounter = 0;
void erase_screen()
{
    if(erasecounter++ > 128*8)
    {
        os_timer_disarm(&tim);
        os_timer_setfn(&tim, timfunc, NULL);
        os_timer_arm(&tim, 1,1);
    }
    else
    {
        i2c_writeData(0x0);
        i2c_checkForAck();
    }

}
