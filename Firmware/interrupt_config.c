/* This file will deal this the interrupt handling and dealing with the
button on U0_RXD pin
*/

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
#include "interrupt_config.h"
#include "interface.h"

LOCAL os_timer_t buttonTapHelper; //timer to run the postButtonTapTimerFunction() function
LOCAL os_timer_t interruptEnableTimer; //timer to enable interrupts some time after the press is detected

/* This timer function will run to enable int GPIO interrupts
 * This function is used for debouncing purposes
*/
void ICACHE_FLASH_ATTR
InterruptConfig_gpioInterruptEnabler()
{
    ETS_GPIO_INTR_ENABLE();
}

/* This timer will be armed for a second at a GPIO interrupt
 * It will wait a second, and then check if the button is still pressed or not
 * The buttonPressHandler function will be called with a press type (short or long)
depending on this
*/
void ICACHE_FLASH_ATTR
InterruptConfig_postButtonTapTimerFunction()
{
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS); //read the gpio interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status); //clearing interrupt

    os_timer_disarm(&interruptEnableTimer); //disarming os_timer_t interruptEnableTimer
    os_timer_setfn(&interruptEnableTimer, (os_timer_func_t *)InterruptConfig_gpioInterruptEnabler, NULL); //setting the function to run
    os_timer_arm(&interruptEnableTimer, 200, 0); //arm interruptEnableTimer to run in 1000ms, non repeating

    if(!(gpio_input_get() & BIT3)) //if the bit is not set, its a LONG_PRESS, a SHORT_PRESS otherwise
        Interface_buttonPressHandler(LONG_PRESS);
    else
        Interface_buttonPressHandler(SHORT_PRESS);
}

/* This is the interrupt handler function
 * On each interrupt, this function will be called
 * This function will disable interrupts
 * Then it will arm os_timer_t buttonTapHelper to call a function postButtonTapTimerFunction
to ascertain if the press was a long one or short one
*/
void ICACHE_FLASH_ATTR
InterruptConfig_gpioInterruptHandlerFunction()
{
    ETS_GPIO_INTR_DISABLE(); //disables interrupts until h
    os_timer_disarm(&buttonTapHelper); //disarming os_timer_t buttonTapHelper
    os_timer_setfn(&buttonTapHelper, (os_timer_func_t *)InterruptConfig_postButtonTapTimerFunction, NULL); //setting the function to run
    os_timer_arm(&buttonTapHelper, 1000, 0); //arm buttonTapHelper to run in 1000ms, non repeating
}

void ICACHE_FLASH_ATTR
InterruptConfig_setupInterrupt()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3); //setting U0RXD as GPIO3
    gpio_output_set(0,0,0,BIT3); //setting GPIO3 as input
    ETS_GPIO_INTR_DISABLE(); //disables GPIO interrupts
    gpio_pin_intr_state_set(GPIO_ID_PIN(3), GPIO_PIN_INTR_NEGEDGE); //trigger interrupt on a positive edge
    ETS_GPIO_INTR_ATTACH((ets_isr_t)InterruptConfig_gpioInterruptHandlerFunction, NULL);//this attaches "gpio_interrupt_handler" as the gpio interrupt handler function
    ETS_GPIO_INTR_ENABLE(); //gpio interrupt enable
}
