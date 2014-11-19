/*******************************************************************************
*
* File Name:   lcd.h
* Project  :   ASURO_EXT_LCD extension-module
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   27.09.2004   Martin Hofmann	 build
* 2.00     04.10.2005	Martin Hofmann	 small changes; uses other i2c routine
* 3.00	   28.09.2006   Martin Hofmann	 pcf8574 routines are now separate in pcf8574.c/.h
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
 
#ifndef LCD_H
#define LCD_H

#include <avr/pgmspace.h>

//Definitionen für Displaygröße
#define LCD_START_LINE1  0x00     		/* DDRAM Adresse des ersten Zeichens in Zeile 1 */
#define LCD_START_LINE2  0x40     		/* DDRAM Adresse des ersten Zeichens in Zeile 2 */




//Vordefinierte Steuerkommandos zur Nutzung mit lcd_command(..)

#define LCD_ENTRY_DEC            0x04   /* Cursor automatisch nach links schieben 								*/
#define LCD_ENTRY_DEC_SHIFT      0x05   /* Cursor automatisch nach links schieben, Display nach rechts schieben */
#define LCD_ENTRY_INC            0x06   /* Cursor automatisch nach rechts schieben 								*/
#define LCD_ENTRY_INC_SHIFT      0x07   /* Cursor automatisch nach rechts schieben, Display nach links schieben */

#define LCD_DISP_OFF             0x08   /* Display aus                           								*/
#define LCD_DISP_ON              0x0C   /* Display an			               								    */
#define LCD_DISP_ON_CURSOR       0x0E   /* Display an, Cursor an                  								*/
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   /* Display an, Cursor blinkend            								*/

#define LCD_MOVE_CURSOR_LEFT     0x10   /* bewegt den Cursor nach links           								*/
#define LCD_MOVE_CURSOR_RIGHT    0x14   /* bewegt den Cursor nach rechts          								*/
#define LCD_MOVE_DISP_LEFT       0x18   /* schiebt den Displayinhalt nach links   								*/
#define LCD_MOVE_DISP_RIGHT      0x1C   /* schiebt den Displayinhalt nach rechts  								*/



void lcd_init(unsigned char dispAttr);

void lcd_clrscr(void);

void lcd_home(void);

void lcd_gotoxy(unsigned char x, unsigned char y);

void lcd_putc(char c);

void lcd_puts(const char *s);

void lcd_puts_p(const char *progmem_s);
#define lcd_puts_P(__s)         lcd_puts_p(PSTR(__s))

void lcd_command(uint8_t cmd);

// Makros für die drei Taster 
#define LCD_KEY_YELLOW   (PIND&(1<<PD6))
#define LCD_KEY_RED      (PIND&(1<<PD2))
#define LCD_KEY_BLUE     (PINB&(1<<PB3))

// globale Variable, die den LED Status speichert
unsigned char LEDstatus; 		// 1= OFF 0=ON    

void lcd_LED(unsigned char stat);

void lcd_setchar(unsigned char AsciiCode, const char *progmem_s);

#endif  /* LCD_H */

