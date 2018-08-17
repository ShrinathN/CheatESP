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
#include "timerkeeping.h"

LOCAL os_timer_t timekeepingTimer;

void ICACHE_FLASH_ATTR
Timekeeping_realTimeIncrementFunction()
{
    realTime++;
    system_soft_wdt_feed(); //watchdog feed every second
#ifdef DEBUG_ENABLE
    os_printf("%s\n", sntp_get_real_time(realTime));
#endif
}

/* This function will increment the RTC time every second
 * will also print the time through the UART_TX if debugging is enabled
*/
void ICACHE_FLASH_ATTR
Timekeeping_sntpTimeRetriever()
{
    realTime = sntp_get_current_timestamp() + 1800;
    os_timer_setfn(&timekeepingTimer, (os_timer_func_t*)Timekeeping_realTimeIncrementFunction, NULL);
    os_timer_arm(&timekeepingTimer, 1000, REPEAT);
}

//sntp setup, everything is self explanatory
void ICACHE_FLASH_ATTR
Timekeeping_setupSNTP()
{
    sntp_stop();
    sntp_setservername(0, "0.in.pool.ntp.org");
    sntp_setservername(1, "1.asia.pool.ntp.org");
    sntp_set_timezone(+5);
    sntp_init();
    os_timer_disarm(&timekeepingTimer);
    os_timer_setfn(&timekeepingTimer, (os_timer_func_t*)Timekeeping_sntpTimeRetriever, NULL);
    os_timer_arm(&timekeepingTimer, 5000, NO_REPEAT); //5 second wait, for the SNTP client to finish intializing
}
