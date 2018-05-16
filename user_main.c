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

                         0xA8,//setting mux ratio
                         0x3F,//64mux, reset 0b00111111

                         0xD3,//set vertical shift by com
                         0x00,//00h, reset

                         0x40,//set display start line as 00, reset
                         0xA1,//segment remap, column address 127 mapped to SEG0
                         0xC8,//remapped mode, scan COM ports in reverse

                         0xDA,//set COM pin hardware config
                         0x12,//alternative COM pin config

                         0x81,//set contrast
                         0x7F,//reset

                         0xA4,//resume to ram content display, reset
                         0xA6,//normal display, reset

                         0xD5,//set display clock
                         0xf0,//not reset

                         0x8D,//charge pump
                         0x14,//enable charge pump during display on

                         0xAF,//turn on display
                        };
uint8 byteToWrite;
unsigned char setFullRange[11] = {0x00,//control byte, following transmission will contain command data bytes

                                 0x00,
                                 0x10,

                                 0x21,//set column start and end address
                                 0x00,//start address, reset
                                 0x7F,//end address, return

                                 0x22,//set page start and end address
                                 0x00,//start address, reset
                                 0x07,//end address reset

                                 0x20,
                                 0x00
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
    if(count == 25)
    {
        i2c_writeData(0x0);
        i2c_checkForAck();
        i2c_writeData(0x0);
        i2c_checkForAck();
        i2c_writeData(0x0);
        i2c_checkForAck();
    }
    if(count > 25)
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
