/*******************************************************************************
*
* File Name:   aufgabe3.c
*
* Lösung zur Aufgabe <ASURO bekommt ein Gesicht> aus Mehr Spaß Mit ASURO, Band II
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

const unsigned char Eye_L[8] PROGMEM = {0X00,0X00,0X1C,0X00,0X04,0X00,0X01,0X03};					  
const unsigned char Eye_R[8] PROGMEM = {0X00,0X00,0X07,0X00,0X04,0X00,0X10,0X18};

const unsigned char MouthA[8] PROGMEM = {0X00,0X00,0X02,0X01,0X00,0X00,0X00,0X00};
const unsigned char MouthB[8] PROGMEM = {0X03,0X01,0X00,0X00,0X18,0X07,0X00,0X00};
const unsigned char MouthC[8] PROGMEM = {0X18,0X10,0X00,0X00,0X03,0X1C,0X00,0X00};
const unsigned char MouthD[8] PROGMEM = {0X00,0X00,0X08,0X10,0X00,0X00,0X00,0X00};


void smily_load(void)		//Nötige Sonderzeichen in das Display laden
{
	lcd_setchar(0,Eye_L);
	lcd_setchar(1,Eye_R);

	lcd_setchar(4,MouthA);
	lcd_setchar(5,MouthB);
	lcd_setchar(6,MouthC);
	lcd_setchar(7,MouthD);
}

void smily_draw(void)		//Smily mit Sonderzeichen auf dem Display darstellen
{
	lcd_home();
	lcd_gotoxy(2,0);
    lcd_putc('(');			//linkes Ohr
	lcd_putc(0);			//linkes Auge
	lcd_putc(1);			//rechtes Auge
	lcd_putc(')');			//rechtes Ohr

	lcd_gotoxy(2,1);
	lcd_putc(4);			//Mund A
	lcd_putc(5);			//Mund B	
	lcd_putc(6);			//Mund C
	lcd_putc(7); 			//Mund D
}

int main(void)
{
	Init();										//Initialisiert ASURO
	lcd_init(LCD_DISP_ON);			//Initialisiert das Display Modul
	
	smily_load();
	smily_draw();
	
	for(;;);
	return 0;
}


