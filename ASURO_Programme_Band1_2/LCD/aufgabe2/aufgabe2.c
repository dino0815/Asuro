/*******************************************************************************
*
* File Name:   aufgabe2.c
*
* Lösung zur Aufgabe <Die Taster> aus Mehr Spaß Mit ASURO, Band II
*
* Project  :   ASURO-LCD extension-module
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00		30.5.06		martin hofmann		build
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

#include "asuro.h"
#include "lcd.h"

int main(void)
{
	Init();										//Initialisiert ASURO
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);			//Initialisiert das Display Modul
	
	for (;;) 
	{
		if (LCD_KEY_BLUE) {
			lcd_clrscr();
			lcd_puts_P("blau");
			while(LCD_KEY_BLUE);
		}
		if (LCD_KEY_YELLOW) {
			lcd_clrscr();
			lcd_puts_P("gelb");
			while(LCD_KEY_YELLOW);
		}
		if (LCD_KEY_RED) {
			lcd_clrscr();
			lcd_puts_P("rot");
			while(LCD_KEY_RED);
		}
	}
	return 0;
}


