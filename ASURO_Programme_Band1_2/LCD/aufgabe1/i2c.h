/*******************************************************************************
*
* File Name:   i2c.h
* Project  :   ASURO_EXT_LCD
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   14.08.2005   Martin Hofmann	 build
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
#ifndef _I2C_H
#define _I2C_H   1

#include <avr/io.h>

/* define the data direction in i2c_start(),i2c_rep_start() */
#define I2C_READ    1
#define I2C_WRITE   0


/**
 @brief initialize the I2C master interface. Need to be called only once 
 @param  void
 @return none
 */
void i2c_init(void);


/** 
 @brief Terminates the data transfer and releases the I2C bus 
 @param void
 @return none
 */
void i2c_stop(void);


/** 
 @brief Issues a start condition and sends address and transfer direction 
  
 @param    addr address and transfer direction of I2C device
 @retval   0   device accessible 
 @retval   1   failed to access device 
 */
unsigned char i2c_start(unsigned char addr);


/**
 @brief Issues a repeated start condition and sends address and transfer direction 

 @param   addr address and transfer direction of I2C device
 @retval  0 device accessible
 @retval  1 failed to access device
 */
unsigned char i2c_rep_start(unsigned char addr);


/**
 @brief Send one byte to I2C device
 @param    data  byte to be transfered
 @retval   0 write successful
 @retval   1 write failed
 */
unsigned char i2c_write(unsigned char data);



/** 
 @brief    read one byte from the I2C device
 
 @param    ack 1 send ack, request more data from device
               0 send nak, read is followed by a stop condition 
 @return   byte read from I2C device
 */
unsigned char i2c_read(unsigned char ack);


#endif
