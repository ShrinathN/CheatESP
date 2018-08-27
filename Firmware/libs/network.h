#ifndef NETWORK_H
#define NETWORK_H

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
#include "oled_functions.h"

#define PACKET_TYPE 0x0
#define IP_BYTEORDER_FIX(a,b,c,d) ((uint32)((d) & 0xff) << 24) | \
    ((uint32)((c) & 0xff) << 16) | \
    ((uint32)((b) & 0xff) << 8)  | \
    (uint32)((a) & 0xff)

static OledStringPtr connectedOledString[9] = {0x02, 0x0e, 0x0d, 0x0d, 0x04, 0x02, 0x13, 0x04, 0x03};

//use this struct to communicate with the socket from outside the header file
struct espconn * esp;

//function definitions
void Network_SocketDataRecvCallbackFunction(void * , char *, unsigned short);
void Network_SocketConnectCallbackFunction(void *);
void Network_connectToServer(void);
void Network_WifiEventHandlerCallbackFunction(System_Event_t *);
void Network_SetupNetwork(void);
#endif
