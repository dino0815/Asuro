// *******************************************
// * Ultraschall für ASURO                   *
// * Roboter versucht Hindernissen           *
// * auszuweichen                            *
// * Achtung: asuro.c ist geändert           *
// * (c) 2005Robin Gruber                    *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band I"           *
// *******************************************

#include "asuro_us.h"


void LocalInit(void)
{
	// Change Oscillator-frequency to 40kHz, no toggling of IO-pin
	TCCR2 = (1 << WGM21) | (1 << CS20);
	OCR2  = 0x64; // 40kHz @8MHz
	// ADC off
	ADCSRA = 0;  
	
	// Analog Comparaot
	ACSR = 0x02; // Generate Interrupt on falling Edge
	ADMUX=0x03; // Multiplexer for Comparator to ADC pin 3
	SFIOR|=(1<<ACME);
	
	DDRD&=~(1<<6);
	
}


void Ping(unsigned char length)
{
	count72kHz=0;
	TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS20);
	while (count72kHz<length) {
		OCR2=0x64+length/2-count72kHz;
	};
	TCCR2 = (1 << WGM21) | (1 << CS20);
	OCR2=0x64;

}


int main(void)
{
	int pos;
	int posmarker;
	int odata[2];
	Init();
	LocalInit();
	while(1) {
//		OdometrieData(odata);
		posmarker=0;
		Ping(20);
		for (pos=0; pos<100; pos++) {
			Sleep(10);
			if ((ACSR&(1<<ACI))!=0) {
				if (posmarker==0) {posmarker=pos;}
			} 
			ACSR|=(1<<ACI);
		}
		if (posmarker>10) {
			StatusLED(GREEN);
			MotorDir(FWD,FWD);
			MotorSpeed(200,200);
		} else {
			StatusLED(RED);
			MotorDir(FWD,RWD);
			MotorSpeed(0,200);
			Sleep(20000);
		}
	}
	return 0;
}

