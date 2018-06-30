// This file has the soft i2c implementation for ESP8266
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

#define I2C_SCL_PIN 0
#define I2C_SCL_MUX PERIPHS_IO_MUX_GPIO0_U
#define I2C_SCL_FUNC FUNC_GPIO0

#define I2C_SDA_PIN 2
#define I2C_SDA_MUX PERIPHS_IO_MUX_GPIO2_U
#define I2C_SDA_FUNC FUNC_GPIO2

#define I2C_SLEEP_US 3

LOCAL void ICACHE_FLASH_ATTR
i2c_scl_set(uint8 status)
{
    if(status & 0x1)
    {
	gpio_output_set(1 << I2C_SCL_PIN, 0, 1 << I2C_SCL_PIN, 0);
    }
    else
    {
	gpio_output_set(0, 1 << I2C_SCL_PIN, 1 << I2C_SCL_PIN, 0);
    }
}

LOCAL void ICACHE_FLASH_ATTR
i2c_sda_set(uint8 status)
{
    if(status & 0x1)
    {
	gpio_output_set(1 << I2C_SDA_PIN, 0, 1 << I2C_SDA_PIN, 0);
    }
    else
    {
	gpio_output_set(0, 1 << I2C_SDA_PIN, 1 << I2C_SDA_PIN, 0);
    }
}

void ICACHE_FLASH_ATTR
i2c_init()
{
    ETS_GPIO_INTR_DISABLE();
    PIN_FUNC_SELECT(I2C_SCL_MUX, I2C_SCL_FUNC);
    PIN_FUNC_SELECT(I2C_SDA_MUX, I2C_SDA_FUNC);

    GPIO_REG_WRITE(
		GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SDA_PIN)),
		GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SDA_PIN))) |
		GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
		);

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SDA_PIN));

    //Set SCK as open drain
    GPIO_REG_WRITE(
		GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCL_PIN)),
		GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCL_PIN))) |
		GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
		);

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SCL_PIN));
    ETS_GPIO_INTR_ENABLE();
    i2c_scl_set(1);
    i2c_sda_set(1);
    return;
}

void ICACHE_FLASH_ATTR
i2c_startCondition()
{
    i2c_scl_set(1);
    i2c_sda_set(1);

    os_delay_us(I2C_SLEEP_US);
    i2c_sda_set(0);

    os_delay_us(I2C_SLEEP_US);
    i2c_scl_set(0);

    os_delay_us(I2C_SLEEP_US);
}

void ICACHE_FLASH_ATTR
i2c_stopCondition()
{
    i2c_scl_set(0);
    i2c_sda_set(0);

    os_delay_us(I2C_SLEEP_US);
    i2c_scl_set(1);

    os_delay_us(I2C_SLEEP_US);
    i2c_sda_set(1);

    os_delay_us(I2C_SLEEP_US);
}

uint8 ICACHE_FLASH_ATTR
i2c_checkForAck()
{
    uint8 ack_status;
    i2c_sda_set(1);

    os_delay_us(I2C_SLEEP_US);
    i2c_scl_set(0);

    os_delay_us(I2C_SLEEP_US);
    i2c_scl_set(1);

    os_delay_us(I2C_SLEEP_US);
    ack_status = GPIO_INPUT_GET(GPIO_ID_PIN(I2C_SDA_PIN));

    os_delay_us(I2C_SLEEP_US);
    i2c_scl_set(0);

    os_delay_us(I2C_SLEEP_US);
    i2c_sda_set(0);

    os_delay_us(I2C_SLEEP_US);
    return (ack_status ? 0 : 1);
}

void ICACHE_FLASH_ATTR
i2c_writeData(uint8 data)
{
    uint8 current_data_bit;
    sint8 counter;
    os_delay_us(I2C_SLEEP_US);
    for(counter = 7; counter >= 0; counter--)
    {
	current_data_bit = (data >> counter);
	i2c_sda_set(current_data_bit);

	os_delay_us(I2C_SLEEP_US);
	i2c_scl_set(1);

	os_delay_us(I2C_SLEEP_US);
	i2c_scl_set(0);

	os_delay_us(I2C_SLEEP_US);
    }
}
