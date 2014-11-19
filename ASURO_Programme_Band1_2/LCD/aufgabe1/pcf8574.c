/*******************************************************************************
*
* File Name:   lcd.c
* Project  :   ASURO_EXT_LCD extension-module
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   27.09.2004   Martin Hofmann	 build
* 2.00     04.10.2005	Martin Hofmann	 small changes; uses other i2c routine
*
*
* Copyright (c) 2007 Martin Hofmann
*****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 ***************************************************************************/
#include "i2c.h"
#include <avr/pgmspace.h>
#include "asuro.h"

#define pcf8574_address 0x40					// I2C Adresse des PCF8574



//sendet ein Byte an den PCF8574
void set_pcf8574(unsigned char byte)
{
	i2c_start(pcf8574_address+I2C_WRITE);		    // Sende Start Condition
    i2c_write(byte);                       // Sende BYte
    i2c_stop();                            // Sende Stop Condition, Bus freigeben
}
