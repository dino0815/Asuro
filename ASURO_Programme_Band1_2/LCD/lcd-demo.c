/*******************************************************************************
*
* File Name:   lcd-demo.c
* Project  :   LCD-DEMO for ASURO-LCD extension-module
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00		17.9.05		martin hofmann		build
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

const unsigned char textstring[4] PROGMEM = "RED";		//definiert einen String im FLASH
const unsigned char special_char[8] PROGMEM = {0X00,0X0E,0X15,0X17,0X11,0X0E,0X00,0X00}; 
//definiert ein Sonderzeichen und speichert dieses im FLASH ab.

int main(void)
{
	Init();										//Initialisiert ASURO
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);			//Initialisiert das Display Modul
	
	lcd_putc('P');								//Gibt ein ASCII Zeichen aus
	lcd_puts("ress");							//Gibt String aus dem RAM aus
	lcd_gotoxy(1,1);							//Setzt Cursor auf 2. Zeichen in der 2. Zeile
	lcd_puts_p(textstring);						//Gibt String aus dem FLASH aus
	lcd_command(LCD_MOVE_CURSOR_RIGHT);			//Bewegt den Cursor nach rechts
	lcd_puts_P("Btn");							//Gibt String aus dem FLASH aus
	
	while(!LCD_KEY_RED);						//Warte, bis roter Taster gedrückt
	while(LCD_KEY_RED);						//Warte, bis roter Taster wieder losgelassen
	
	lcd_command(LCD_MOVE_DISP_RIGHT);			//Verschiebe Display
	
	while (!LCD_KEY_BLUE);						//Warte, bis blauer Taster gedrückt
	
	lcd_LED(ON);								//Hintergrundbeleuchtung an
	
	while (!LCD_KEY_YELLOW);					//Warte, bis gelber Taster gedrückt
	
	lcd_clrscr();								//Displayinhalt leeren
	lcd_setchar(3,special_char);				//Oben definiertes Sonderzeichen ins Display laden und dem ASCII Zeichen 3 zuordnen
	lcd_gotoxy(4,0);							//Setzt Cursor auf 5. Zeichen in der 1. Zeile
	lcd_putc(3);								//ASCII Zeichen 3 ausgeben. 
	
	for(;;);
	return 0;
}


