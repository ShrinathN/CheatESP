#include "user_config.h"
#include "i2c.h"
#include "oled.h"
#include "oled_functions.h"
#include "fonts.h"
#define AP_SSID ""
#define AP_PASSWORD ""
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)
void ICACHE_FLASH_ATTR func();
void timfunc();
void erase_screen();

uint8 byteToWrite;
LOCAL os_timer_t tim;
void user_init(void)
{
    os_delay_us(500000); //starts working the magic 0.5s after booting
    func(); //screen testing function, dumps all the fonts
}
/*
uint16 count;
void ICACHE_FLASH_ATTR OledTestFunction()
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
*/
