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
#include "i2c.h"
#include "pcf8574.h"
#include <avr/pgmspace.h>
#include "asuro.h"

#define RS_bit 	  	    0				//Register Select Bit  ( high for data, low for command)
#define RW_bit		    1				//H: Read    / L: Write
#define E_bit 			2				//Enable Bit
#define LED_bit    		3				//H: LED Off / L: LED on

#define LCD_START_LINE1  0x00     		/* DDRAM Adresse des ersten Zeichens in Zeile 1 */
#define LCD_START_LINE2  0x40     		/* DDRAM Adresse des ersten Zeichens in Zeile 2 */

unsigned char LEDstatus = 1;			// 1= OFF 0=ON    //globale Varialble, die den LED Status enthält

/*****************************************************************************/
/* Low Level Functions: */
/*****************************************************************************/


//Sendet ein Halbbyte (4 Bit) mit Enableflanke an das Display. 
//Dabei immer LEDstatus Bit mitsenden
void send_nibble(unsigned char i2cbyte)
{
	set_pcf8574(i2cbyte|(LEDstatus<<LED_bit));				//Daten anlegen
	set_pcf8574(i2cbyte|(LEDstatus<<LED_bit)|(1<<E_bit));	//Enable_bit setzen
	set_pcf8574(i2cbyte|(LEDstatus<<LED_bit));				//Enable_bit löschen
}

// Sendet Steuerbyte, getrennt in zwei Halbbytes
void lcd_command(unsigned char byte)
{
	send_nibble(byte&0xF0);  		// sende oberes Halbbyte
	send_nibble(byte<<4);			// sende unteres Halbbyte
}

// Sendet Datenbyte, getrennt in zwei Halbbytes
void lcd_putc(unsigned char byte)
{
	send_nibble((byte&0xF0)|(1<<RS_bit)); 		// sende oberes Halbbyte, dabei RS_bit gesetzt
	send_nibble((byte<<4)  |(1<<RS_bit)); 		// sende unteres Halbbyte, dabei RS_bit gesetzt
}

// Hintergrundbeleuchtung schalten
void lcd_LED(unsigned char stat)
{
	LEDstatus = (~stat)&0x01;				//globale Variable LEDstatus aktualisieren
	set_pcf8574(LEDstatus<<LED_bit);		//aktuellen LED Status ans Display senden
}


/*****************************************************************************/
/* High Level Funktionen: */
/*****************************************************************************/


/*************************************************************************
Initialisiere I2C Bus, LCD Display, und die drei Taster	
Input:  Cursor-Typ (siehe lcd.h)
Returns:  keine
*************************************************************************/
void lcd_init (unsigned char dispAttr)
{
	i2c_init();                             // I2C Bus initialisieren
	
	TCCR2 = (1 << WGM21) | (1 << CS20);   //  OC2 PIN deaktivieren, aber 36kHz Timer weiterlaufen lassen (z.B. für Sleep(void) )

	DDRD &= ~((1<<PD2)|(1<<PD6));			// roten und gelben Taster als Eingang definieren
	DDRB &= ~(1<<PB3);						// blauen Taster als Eingang definieren
	
	
	LEDstatus=0x01;							// LEDstatus Variable setzen (LED ist jetzt aus)

	send_nibble(0x28);							// (Function Set) noch im 8-Bit Modus -> in 4 Bit Modus umschalten  
	
	lcd_command(0x28);						// (Function Set) 4 Bit, 2 Zeilen, 5x7 Schriftart

	lcd_command(dispAttr);					// (Display ON/OFF) 

	lcd_command(0x01);						// (Clear Display)

	lcd_command(0x06);						// (Entry Mode Set) Cursor Auto-Increment

}

/*************************************************************************
Cursor auf vorgegebene Position setzen
Input:    x  horizontale Position  (0= ganz links)
          y  vertikale Position    (0= erste Zeile)
Returns:  keine
*************************************************************************/
void lcd_gotoxy(unsigned char x, unsigned char y)
{
    if ( y==0 ) 
        lcd_command(0x80+LCD_START_LINE1+x);
    else
        lcd_command(0x80+LCD_START_LINE2+x);
}



/*************************************************************************
Display löschen und Cursor auf Ausgangsposition setzen
Input:  keine
Returns:  keine
*************************************************************************/
void lcd_clrscr(void)
{
	lcd_command(0x01);
	Sleep(82);   //~1ms		//important!!!
}


/*************************************************************************
Cursor auf Ausgangsposition setzen
Input:  keine
Returns:  keine
*************************************************************************/
void lcd_home(void)
{
	lcd_command(0x02);
	Sleep(82);   //~1ms		//wichtig!!!
}



/*************************************************************************
String auf dem Display ausgeben
Input:    String aus dem RAM
Returns:  keine
*************************************************************************/
void lcd_puts(const char *s)
{
    register char c;

    while ( (c = *s++) ) {
        lcd_putc(c);
    }
}


/*************************************************************************
String auf dem Display ausgeben 
Input:     String aus dem Program Memory (Flash)                                     
Returns:   keine
*************************************************************************/
void lcd_puts_p(const char *progmem_s)
{
    register char c;

    while ( (c = PRG_RDB(progmem_s++)) ) {
        lcd_putc(c);
    }

}
#define lcd_puts_P(__s)         lcd_puts_p(PSTR(__s))


/*************************************************************************
Sonderzeichen an das Display senden
Input:     AsciiCode des zu erzeugenden Sonderzeichens
           Pointer auf die Definition des Zeichens
Returns:   keine
*************************************************************************/
void lcd_setchar(unsigned char AsciiCode, const char *progmem_s)
{
	unsigned char i;
	
	lcd_command( 0x40|(AsciiCode<<3));
	for(i=0;i<=7;i++)
	{
		lcd_putc(pgm_read_byte(progmem_s++));
	}
}


