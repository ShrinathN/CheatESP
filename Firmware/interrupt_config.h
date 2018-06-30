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

//comment when compiling
//#include "user_config.h"

//this struct will keep a record of some details about the timer and button presses
struct _timerStatus_
{
    uint8 timerRunning : 1; //this variable is 1 when the timer is running and 0 otherwise
    uint8 pulsesLastSecond : 7; //the number of button presses in a second
} timerStatus;
LOCAL os_timer_t buttonTapHelper; //timer to run the postButtonTapTimerFunction() function

/* This timer will be armed for a second at the first of a GPIO interrupt
 * It will wait a second, and then call a function which will act according to the
variable pulsesPerSecond
 * At the end of this function timerStatus.timerRunning will be set to 0
*/
void ICACHE_FLASH_ATTR
postButtonTapTimerFunction()
{
    timerStatus.timerRunning = 0; //setting as 0 to indicate timer stop
    //function to handle button presses per second, can be found in the interface.h file
    buttonPressHandler(timerStatus.pulsesLastSecond);

}

/* This is the interrupt handler function
 * On each interrupt, this function will be called
 * It will first check if the timer is running or not
by checking if timerStatus.timerRunning is non zero
 * If the timer is not running (=0), the timer will be
started for a second, non repeating using os_timer_t buttonTapHelper
 * If the timer is running (!=0), every interrupt will
increment timerStatus.pulsesLastSecond counter
 * The timer function will call a function to take the appropriate
action after a second based on the number of button clicks, and also reset timerRunning,
and pulsesLastSecond parameters of timerStatus struct to 0
*/
void ICACHE_FLASH_ATTR
gpioInterruptHandlerFunction()
{
    os_delay_us(50*1000); //50ms delay after press for debouncing purpose, EXPERIMENTAL
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS); //read the gpio interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status); //clearing interrupt
    if(timerStatus.timerRunning) //if the timer is running, increment pulses
    {
        timerStatus.pulsesLastSecond++;//incrementing the number of pulses at every run of this
    }
    else //if the timer wasn't running already, start it
    {
        timerStatus.timerRunning = 1; //setting this as 1 because the timer is running
        timerStatus.pulsesLastSecond = 0; //reseting pulsesPerSecond counter
        os_timer_disarm(&buttonTapHelper); //disarming os_timer _t
        os_timer_setfn(&buttonTapHelper, postButtonTapTimerFunction, NULL); //setting the function to run
        os_timer_arm(&buttonTapHelper, 1000, 0); //arm buttonTapHelper to run in 1000ms, non repeating
    }
}

void ICACHE_FLASH_ATTR
setupInterrupt()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3); //setting U0RXD as GPIO3
    gpio_output_set(0,0,0,BIT3); //setting GPIO3 as input
    ETS_GPIO_INTR_DISABLE(); //disables GPIO interrupts
    gpio_pin_intr_state_set(GPIO_ID_PIN(3), GPIO_PIN_INTR_NEGEDGE); //trigger interrupt on a positive edge
    ETS_GPIO_INTR_ATTACH(gpioInterruptHandlerFunction, NULL);//this attaches "gpio_interrupt_handler" as the gpio interrupt handler function
    ETS_GPIO_INTR_ENABLE(); //gpio interrupt enable
    timerStatus.timerRunning = 0; //setting both parameters as 0
    timerStatus.pulsesLastSecond = 0;
}
