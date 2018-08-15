#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// has a list of all files to include
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
#include "airkiss.h"
#include "at_custom.h"
#include "c_types.h"
#include "c_types.h.orig"
#include "eagle_soc.h"
#include "esp_sdk_ver.h"
#include "espnow.h"
#include "ets_sys.h"
#include "gpio.h"
#include "ip_addr.h"
#include "mem.h"
#include "mesh.h"
#include "os_type.h"
#include "osapi.h"
#include "ping.h"
#include "pwm.h"
#include "queue.h"
#include "simple_pair.h"
#include "smartconfig.h"
#include "sntp.h"
#include "spi_flash.h"
#include "upgrade.h"
#include "user_interface.h"
#include "espconn.h"
#include "wpa2_enterprise.h"

//===============MACROS===============
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)
//====================================


//===============GLOBAL DEFINES===============
#define OFF 0
#define ON 1
#define REPEAT 0x1
#define NO_REPEAT 0x0
//============================================


//===============CONFIG===============

#define AP_SSID "iot7"
#define AP_PASSWORD "jehova007" //comment if you're not using a password
#define SCREEN_TIMEOUT_MS 5000
//#define DEBUG_ENABLE 0 //uncomment to enable debug
//====================================

#endif
