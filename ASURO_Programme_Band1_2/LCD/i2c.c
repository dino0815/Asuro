/*******************************************************************************
*
* File Name:   i2c.c
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
#include <avr/io.h>


// Pin Definitionen:
#define SDA     	2				// SDA Port C, Pin 2     ADC2 
#define SCL		3				// SCL Port C, Pin 3	 ADC3
#define SDA_DDR		DDRC
#define SCL_DDR		DDRC
#define SDA_PORT	PORTC
#define SCL_PORT	PORTC
#define SDA_PIN		PINC
#define SCL_PIN		PINC

//Funktionsprototypen:
unsigned char i2c_write( unsigned char data );


/*************************************************************************
	kurze Pause
*************************************************************************/
volatile void i2c_delay(void)
{
	volatile unsigned char i;
	for (i=0;i<3;i++) {};
}


/*************************************************************************
	Initialisierung des I2c Busses
*************************************************************************/ 
void i2c_init(void)
{
    cbi(SDA_DDR, SDA);  // SDA als Eingang, also hochohmig schalten
    cbi(SCL_DDR, SCL);  // SCL als Eingang, also hochohmig schalten
    cbi(SDA_PORT, SDA); // SDA auf low
    cbi(SCL_PORT, SCL); // SCL aus low
}

/*************************************************************************	
	Erzeugt eine Start-Condition, sendet Slave Adresse und Datenrichtung
	return 0 = Slave antwortet, 1= Slave antwortet nicht
*************************************************************************/
unsigned char i2c_start(unsigned char addr) 
{
	sbi(SDA_DDR, SDA);					//SDA auf low ziehen
	i2c_delay();						//Pause
	
	return i2c_write(addr);			//Sende Adresse
}


/*************************************************************************
	Erzeugt eine Repeated Start-Condition, sendet Slave ADresse und Datenrichtung
	return 0 = Slave antwortet, 1= Slave antwortet nicht
*************************************************************************/
unsigned char i2c_rep_start(unsigned char addr)
{
	sbi(SCL_DDR, SCL);		//SCL auf low ziehen
	i2c_delay();			//Pause
	cbi(SDA_DDR, SDA);		//SDA loslassen
	i2c_delay();			//Pause
	cbi(SCL_DDR, SCL);		//SCL loslassen
	i2c_delay();			//Pause
	sbi(SDA_DDR, SDA);		//SDA auf low ziehen
	i2c_delay();			//Pause
	
	return i2c_write(addr);	//
}



/*************************************************************************
	Beendet die Übertragung, sendet die Stopcondition und gibt den Bus
	wieder frei
*************************************************************************/
void i2c_stop(void)
{	
	sbi(SCL_DDR, SCL);			//SCL auf low ziehen
	sbi(SDA_DDR, SDA);			//SDA auf low ziehen
	i2c_delay();				//Pause
	cbi(SCL_DDR, SCL);			//SCL loslassen
	i2c_delay();				//Pause
	cbi(SDA_DDR, SDA);			//SDA loslassen
	i2c_delay();				//Pause
}

/*************************************************************************
	Sendet ein Byte
	return 0 = Schreibvorgang erfolgreich
	return 1 = Fehler: Slave hat keine Bestätigung gesendet
*************************************************************************/
unsigned char i2c_write( unsigned char data )
{
	signed char i;
	for (i=7;i>=0;i--) {							//8 mal, für jedes Bit
		sbi(SCL_DDR, SCL);							//SCL auf low ziehen
		
		if (data & (1<<i)) cbi(SDA_DDR, SDA);		//SDA loslassen
		else			    sbi(SDA_DDR, SDA);		//SDA auf low ziehen
		
		i2c_delay();								//Pause
		cbi(SCL_DDR, SCL);							//SCL loslassen
		i2c_delay();								//Pause     
	}
	
	//auf aknowledge warten:
	sbi(SCL_DDR, SCL);								//SCL auf low ziehen
	cbi(SDA_DDR, SDA);								//SDA loslassen
	i2c_delay();									//Pause
	
	cbi(SCL_DDR, SCL);								//SCL loslassen
	while (!(SCL_PIN &(1<<SCL)));					//Warte auf SCL High
	
	if (SDA_PIN & (1<<SDA)) return 1;
	return 0;
}


/*************************************************************************
	Liest ein Byte vom I2C Slave und sendet ggf. ein Acknowledge
	ack=1: Sende Acknowlede und fordere weitere Daten vom Slave an
	ack=0: Kein Acknowledge; der nächste Befehl muss ein i2c_stop sein
*************************************************************************/

unsigned char i2c_read(unsigned char ack)
{
	unsigned char ret=0;
	signed char i;
	
	for (i=7;i>=0;i--) {							//8 mal, für jedes Bit
		sbi(SCL_DDR, SCL);							//SCL auf low ziehen
		cbi(SDA_DDR, SDA);							//SDA loslassen
		i2c_delay();								//Pause
		
		cbi(SCL_DDR, SCL);							//SCL loslassen
		i2c_delay();								//Pause
		ret = (ret<<1);							//zum nächsten Bit schieben
		if (SDA_PIN & (1<<SDA)) {ret=ret|1;};		//Daten speichern
	}
	//Jetzt noch ggf. Acknowledge senden:
	sbi(SCL_DDR, SCL);								//SCL auf low ziehen
	
	if (ack==0) cbi(SDA_DDR, SDA);					//SDA loslassen
	else sbi(SDA_DDR, SDA);						//SDA auf low ziehen
	
	i2c_delay();									//Pause
	cbi(SCL_DDR, SCL);								//SCL  loslassen

	while (!(SCL_PIN &(1<<SCL)));					//Warete auf SCL High
	i2c_delay();									//Pause
	return ret;
}
