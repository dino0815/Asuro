/*******************************************************************************
*
* File Name:   aufgabe1.c
*
* Lösung zur Aufgabe <ASURO "textet"> aus Mehr Spaß Mit ASURO, Band II
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
	Init();								 //Initialisiert ASURO
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);	 //Initialisiert das Display Modul
	
	lcd_puts("Ich bin");				 //Gibt String aus dem RAM aus
	
	lcd_gotoxy(3,1);					 //Setzt Cursor auf 4. Zeichen in der 2. Zeile

	lcd_puts_P("ASURO");				 //Gibt String aus dem FLASH aus
	
	for(;;);
	return 0;
}


