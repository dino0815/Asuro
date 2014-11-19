// *******************************************
// * Snakevision f�r ASURO                   *
// * Verfolgt eine W�rmequelle               *
// *                                         *
// *                                         *
// *                                         *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spa� mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"

// Schwellen f�r die intensit�tsabh�ngige Geschwindigkeit

#define THRESH1 20
#define THRESH2 80
#define THRESH3 200


void ThermalData(unsigned int *data)
{
	// ThermalData() funktioniert genauso, wie LineData(), ...
	// ... nur dass dabei die interne Spannungsreferenz als ...
	// ... AD-Wandlerreferenz verwendet wird.
	
	// Prozessorinterne Referenz verwenden
	// Multiplexer auf linken Sensor schalten
	ADMUX = (1 << REFS0) | (1 << REFS1) | IR_LEFT;	
	// Wandlung starten
	ADCSRA |= (1 << ADSC);
	// Warten, bis Wandlung beendet			        
	while (!(ADCSRA & (1 << ADIF)));	                
	// ADCIF zur�cksetzen
	ADCSRA |= (1 << ADIF);			        
	// ADC-Wert auslesen
	data[0] = ADCL + (ADCH << 8);

	// Prozessorinterne Referenz verwenden
	// Multiplexer auf rechten Sensor schalten
	ADMUX = (1 << REFS0) | (1 << REFS1) | IR_RIGHT;
	// Wandlung starten
	ADCSRA |= (1 << ADSC);
	// Warten, bis Wandlung beendet		
	while (!(ADCSRA & (1 << ADIF)));	
	// ADCIF zur�cksetzen
	ADCSRA |= (1 << ADIF);
	// ADC-Wert auslesen
	data[1] = ADCL + (ADCH << 8);
}


int main(void)
{
	// Speicher f�r die Messwerte bereitstellen
    	unsigned int tdata[2];
	unsigned int speed;
	signed int diff,sum;
    	Init();
	// Motoren immer auf vorw�rts
	MotorDir(FWD,FWD);
	while(1)
    	{
		// Messwerte einlesen
        	ThermalData(tdata);
		// Die Summe der Werte wird f�r die Berechnung der ...
		// ... Geschwindigkeit genutzt
		sum=tdata[0]+tdata[1];
 		// Signal stark genug?
		if (sum>THRESH1) {
			StatusLED(GREEN);
			speed=140;
			// Ziemlich stark?
			if (sum>THRESH2) {
				StatusLED(YELLOW); 
				speed=200;
			}
			// Sehr stark?
			if (sum>THRESH3) {
				StatusLED(RED); 
				speed=255;
			}
			// Richtung bestimmen
			diff=((signed)tdata[0]-(signed)tdata[1])*32/sum;
			if (diff>4) {
				// Links deutlich w�rmer? Nach links fahren!
				BackLED(ON,OFF); 
				MotorSpeed(0,speed);
			}
			else if (diff<-4) {
				// Rechts deutlich w�rmer? Nach rechts fahren!
				BackLED(OFF,ON); 
				MotorSpeed(speed, 0);
			}
			else {
				// Sonst geradeaus!
				BackLED(OFF,OFF); 
				MotorSpeed(speed, speed);
			}
		}
		else {
			// Keine W�rmequelle in Sicht? Stehen bleiben!
			StatusLED(OFF);
			BackLED(OFF,OFF);
			MotorSpeed(0,0);

		}
    	}     
	return 0;
}

