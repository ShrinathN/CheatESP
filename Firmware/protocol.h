// This file contains everything related to networking, and connectivity aspect
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
//comment while compiling
//#include "user_config.h"
//#include "oled_functions.h"
#define PACKET_TYPE 0x0

//==========CONFIG==========
#define IP_BYTEORDER_FIX(a,b,c,d) ((uint32)((d) & 0xff) << 24) | ((uint32)((c) & 0xff) << 16) | ((uint32)((b) & 0xff) << 8)  | (uint32)((a) & 0xff)
//==========================

enum{
    MESSAGE_PACKET_0 = 0x0,
    MESSAGE_PACKET_1 = 0x1,
    MESSAGE_PACKET_2 = 0x2,
    IMAGE_PACKET_0 = 0x3,
    IMAGE_PACKET_1 = 0x4,
    IMAGE_PACKET_2 = 0x5,
    IMAGE_PACKET_3 = 0x6,
    IMAGE_PACKET_4 = 0x7,
    IMAGE_PACKET_5 = 0x8,
    IMAGE_PACKET_6 = 0x9,
    IMAGE_PACKET_7 = 0xA,
    IMAGE_PACKET_8 = 0xB,
    IMAGE_PACKET_9 = 0xC,
    IMAGE_PACKET_10 = 0xD,
};

//use this struct to communicate with the socket from outside the header file
struct espconn * esp;
/* This function will be called whenver data is received from the server
 * The data will be received in packets of a maximum of 95 bytes, since the callback function will
 *
 *
*/
void ICACHE_FLASH_ATTR
SocketDataRecvCallbackFunction(void * arg, char *dataPointer, unsigned short packetLength)
{
    system_soft_wdt_stop();
    OledStringStruct *tempOledStruct = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
    tempOledStruct->ptr = dataPointer + 1;
    tempOledStruct->len = packetLength - 1;
    //this is the section for a message packet
    if(dataPointer[PACKET_TYPE] == MESSAGE_PACKET_0)
    {
        Oled_eraseScreen();
        Oled_returnCursor();
        Oled_writeString(tempOledStruct); //write all the stuff on the screen
        system_soft_wdt_restart();
    }
    //if the packet is a non-first message packet
    else if((dataPointer[PACKET_TYPE] == MESSAGE_PACKET_1) ||
            (dataPointer[PACKET_TYPE] == MESSAGE_PACKET_2))
    {
        Oled_writeString(tempOledStruct);
    }
    os_free(tempOledStruct);
}

OledStringPtr connectedOledString[9] = {0x02, 0x0e, 0x0d, 0x0d, 0x04, 0x02, 0x13, 0x04, 0x03};

/* This function will be called whenever there is a successful connection from the ESP to the server
 *
*/
void ICACHE_FLASH_ATTR
SocketConnectCallbackFunction(void * arg)
{
    struct espconn * localEsp = (struct espconn *)arg;
    uint8 initByte = 'a';//this is the init message
    espconn_send(localEsp, &initByte, 1); //sends the init message over :)
}

void ICACHE_FLASH_ATTR
connectToServer()
{
#ifdef DEBUG_ENABLE
    os_printf("Running connect routine\n");
#endif
    esp = (struct espconn *)os_zalloc(sizeof(struct espconn)); //allocating space for espconn struct
    esp_tcp *tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp)); //allocating space for esp_tcp struct


    esp->type = ESPCONN_TCP; //setting operation mode as TCP
    esp->proto.tcp = (esp_tcp *)tcp; //setting the info as tcp
    esp->proto.tcp->local_port = espconn_port(); //getting an unused port to use
    esp->proto.tcp->remote_port = 8000; //setting the remote server's port as 8000

    //android's APs' IP is 192.168.43.1
    esp->proto.tcp->remote_ip[0] = 192;
    esp->proto.tcp->remote_ip[1] = 168;
    esp->proto.tcp->remote_ip[2] = 0;
    esp->proto.tcp->remote_ip[3] = 107;

    esp->state = ESPCONN_NONE; //setting the state as none
    espconn_regist_connectcb(esp, SocketConnectCallbackFunction); //registering on connect callback function
    espconn_regist_recvcb(esp, SocketDataRecvCallbackFunction); //registering the data recv callback function
    espconn_connect(esp); //connecting the esp to the server
}


/* WiFi event handler function
 *
*/
void ICACHE_FLASH_ATTR
WifiEventHandlerCallbackFunction(System_Event_t * systemEvent)
{
    if(systemEvent->event == EVENT_STAMODE_GOT_IP) //is executed when an IP is obtained
    {
        OledStringStruct * connectedMessage = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
        connectedMessage->len = 9;
        connectedMessage->ptr = connectedOledString;
        setupSNTP();
        connectToServer(); //call to connect to the server
        Oled_writeString(connectedMessage);
        os_free(connectedMessage);
    }
    else if(systemEvent->event == EVENT_STAMODE_DISCONNECTED) //if the AP has disconnected
        espconn_delete(esp);
}

/* This function will connect the ESP8266 to the WiFi network
 * If a password is set, uncomment the AP_PASSWORD line
*/
void ICACHE_FLASH_ATTR
SetupNetwork()
{
    struct station_config * statCon = (struct station_config *)os_zalloc(sizeof(struct station_config)); //allocating space for the struct
    os_strcpy(statCon->ssid, AP_SSID); //copying the SSID
#ifdef AP_PASSWORD //if the AP_PASSWORD is defined, copy the password
    os_strcpy(statCon->password, AP_PASSWORD); //copying the password
#endif
    wifi_set_opmode_current(STATION_MODE); //setting as station mode
    wifi_station_set_config_current(statCon); //setting config as statCon
    os_free(statCon); //freeing up the space
    wifi_set_event_handler_cb(WifiEventHandlerCallbackFunction);
}
