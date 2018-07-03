/* This file will deal the actual timekeeping operations
 * The ESP will fetch the time using SNTP
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

//uncomment while compiling
#include "user_config.h"
#define REPEAT 0x1

uint32 realTime;
LOCAL os_timer_t timekeepingTimer;

void ICACHE_FLASH_ATTR
timekeepingFunction()
{
    realTime++;
#ifdef DEBUG_ENABLE
    os_printf("%s\n", sntp_get_real_time(realTime));
#endif
}

/* This function will increment the RTC time every second
 * will also print the time through the UART_TX if debugging is enabled
*/
void ICACHE_FLASH_ATTR
initialTimerFunction()
{
    realTime = sntp_get_current_timestamp() + 1800;
    os_timer_setfn(&timekeepingTimer, timekeepingFunction, NULL);
    os_timer_arm(&timekeepingTimer, 1000, REPEAT);
}

//sntp setup, everything is self explanatory
void ICACHE_FLASH_ATTR
setupSNTP()
{
    sntp_stop();
    sntp_setservername(0, "0.in.pool.ntp.org");
    sntp_setservername(1, "1.asia.pool.ntp.org");
    sntp_set_timezone(+5);
    sntp_init();
    os_timer_disarm(&timekeepingTimer);
    os_timer_setfn(&timekeepingTimer, initialTimerFunction, NULL);
    os_timer_arm(&timekeepingTimer, 5000, 0); //5 second wait, to fetch the first time
}
