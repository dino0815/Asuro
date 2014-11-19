// *******************************************
// * Snakevision f�r ASURO                   *
// * Meidet eine W�rmequelle, ein Abgleich   *
// * der Hintergundhelligkeit und des        *
// * Verst�rkeroffsets durch Tastendruck ist *
// * m�glich                                 *
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
	// Speicher f�r die Messwerte und f�r Offset bereitstellen
    	unsigned int tdata[2],offset[2];
	unsigned int speed;
	signed int diff,sum;
    	Init();
	// Port D, Pin 6 (die ehemalige Front-LED), auf Eingang ...
	// ... schalten und den Pullup aktivieren.
	DDRD&=~(1<<6);
	PORTD|=(1<<6);
	// Motoren immer auf r�ckw�rts
	MotorDir(RWD,RWD);
	// Offset mit null initialisieren	
	offset[0]=0;
	offset[1]=0;
	while(1)
    	{
		// Messwerte einlesen
        	ThermalData(tdata);
		// Wenn Knopf gedr�ckt, aktuellen Wert als Offset speichern
		if ((PIND&(1<<6))==0) {
			offset[0]=tdata[0];
			offset[1]=tdata[1];
		}
		// Offset abziehen, aber nur, wenn der Messwert gro� ...
		// ... genug ist
		if (tdata[0]>offset[0]) tdata[0]-=offset[0]; else tdata[0]=0;
		if (tdata[1]>offset[1]) tdata[1]-=offset[1]; else tdata[1]=0;
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
				// Links deutlich w�rmer? Nach links schwenken!
				BackLED(ON,OFF); 
				MotorSpeed(speed, 0);
			}
			else if (diff<-4) {
				// Rechts deutlich w�rmer? Nach rechts schwenken!
				BackLED(OFF,ON); 
				MotorSpeed(0, speed);
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

