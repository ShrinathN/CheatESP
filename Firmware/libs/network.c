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
#include "user_config.h"
#include "network.h"
#include "oled_functions.h"
#include "timerkeeping.h"

void ICACHE_FLASH_ATTR
Network_SocketDataRecvCallbackFunction(void * arg, char *dataPointer, unsigned short packetLength)
{
    static OledStringStruct * networkStringStruct;
    networkStringStruct = os_zalloc(sizeof(OledStringStruct));
    networkStringStruct->len = (OledStringLen)os_strlen(dataPointer);
    networkStringStruct->ptr = (OledStringPtr *)os_zalloc(networkStringStruct->len);
    OledFunctions_stringToOledString(dataPointer,networkStringStruct->ptr);
    OledFunctions_writeString(networkStringStruct);
    os_free(networkStringStruct->ptr);
    os_free(networkStringStruct);
}

/* This function will be called whenever there is a successful connection from the ESP to the server
 *
*/
void ICACHE_FLASH_ATTR
Network_SocketConnectCallbackFunction(void * arg)
{
    struct espconn * localEsp = (struct espconn *)arg;
    uint8 initByte = 'a';//this is the init message
    espconn_send(localEsp, &initByte, 1); //sends the init message over :)
}

void ICACHE_FLASH_ATTR
Network_connectToServer()
{
    esp = (struct espconn *)os_zalloc(sizeof(struct espconn)); //allocating space for espconn struct
    esp_tcp *tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp)); //allocating space for esp_tcp struct


    esp->type = ESPCONN_TCP; //setting operation mode as TCP
    esp->proto.tcp = (esp_tcp *)tcp; //setting the info as tcp
    esp->proto.tcp->local_port = espconn_port(); //getting an unused port to use
    esp->proto.tcp->remote_port = 8000; //setting the remote server's port as 8000

    //android's APs' IP is 192.168.43.1
    //first device connected to IP is 192.168.43.218
    esp->proto.tcp->remote_ip[0] = 192;
    esp->proto.tcp->remote_ip[1] = 168;
    esp->proto.tcp->remote_ip[2] = 43;
    esp->proto.tcp->remote_ip[3] = 218;

    esp->state = ESPCONN_NONE; //setting the state as none
    espconn_regist_connectcb(esp, Network_SocketConnectCallbackFunction); //registering on connect callback function
    espconn_regist_recvcb(esp, Network_SocketDataRecvCallbackFunction); //registering the data recv callback function
    espconn_connect(esp); //connecting the esp to the server
}


/* WiFi event handler function
 *
*/
void ICACHE_FLASH_ATTR
Network_WifiEventHandlerCallbackFunction(System_Event_t * systemEvent)
{
    if(systemEvent->event == EVENT_STAMODE_GOT_IP) //is executed when an IP is obtained
    {
        OledStringStruct * connectedMessage = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
        connectedMessage->len = 9;
        connectedMessage->ptr = connectedOledString;
        Timekeeping_setupSNTP();
//        Network_connectToServer(); //call to connect to the server
        OledFunctions_writeString(connectedMessage);
        os_free(connectedMessage);
    }
    else if(systemEvent->event == EVENT_STAMODE_DISCONNECTED) //if the AP has disconnected
        espconn_delete(esp);
}

/* This function will connect the ESP8266 to the WiFi network
 * If a password is set, uncomment the AP_PASSWORD line
*/
void ICACHE_FLASH_ATTR
Network_SetupNetwork()
{
    struct station_config * statCon = (struct station_config *)os_zalloc(sizeof(struct station_config)); //allocating space for the struct
    os_strcpy(statCon->ssid, AP_SSID); //copying the SSID
#ifdef AP_PASSWORD //if the AP_PASSWORD is defined, copy the password
    os_strcpy(statCon->password, AP_PASSWORD); //copying the password
#endif
    wifi_set_opmode_current(STATION_MODE); //setting as station mode
    wifi_station_set_config_current(statCon); //setting config as statCon
    os_free(statCon); //freeing up the space
    wifi_set_event_handler_cb(Network_WifiEventHandlerCallbackFunction);
}
