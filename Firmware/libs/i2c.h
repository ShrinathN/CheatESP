#ifndef I2C_H
#define I2C_H
// This file has the soft i2c definitions for ESP8266
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

#define I2C_SCL_PIN 0
#define I2C_SCL_MUX PERIPHS_IO_MUX_GPIO0_U
#define I2C_SCL_FUNC FUNC_GPIO0

#define I2C_SDA_PIN 2
#define I2C_SDA_MUX PERIPHS_IO_MUX_GPIO2_U
#define I2C_SDA_FUNC FUNC_GPIO2

#define I2C_SLEEP_US 3

void i2c_scl_set(uint8);
void i2c_sda_set(uint8);
void i2c_init(void);
void i2c_startCondition(void);
void i2c_stopCondition(void);
uint8 i2c_checkForAck(void);
void i2c_writeData(uint8);

#endif
