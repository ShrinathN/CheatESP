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
/* This file will deal with all the functions related to the protocol this watch will follow
 *
 * ===OVERVIEW===
 * 1.The ESP8266 is the "client", will function in STATION_MODE
 * 2.A mobile device will act as the "server", and host the WiFi network
 * 3.The ESP8266 will request the file listing from the server, by connecting to the server on a port and,
 * send the request code.
 * 4.The response will send a packet
 *
 * Since the data receive callback function recommends the received data be ~100 bytes,
 * packets will be limited to 95 bytes at a time. The server software will handle this.
 *
 * ======CODES======
 * All codes are single byte
 *
 * ===FILE LISTING===
 * Code(0x10)
 *
 * ===FILE LISTING RESPONSE===
 * Code(0x10 + {number of files}), {array of filenames separated by a 0x2Bh character}
 * Since the font map is 42 characters, 6 bits are used per character
 *
 * ===FILE REQUEST===
 *
*/
