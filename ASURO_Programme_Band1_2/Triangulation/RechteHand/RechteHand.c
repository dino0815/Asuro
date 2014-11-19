// *******************************************
// * Triangulation für ASURO                 *
// *                                         *
// * Versucht, einen konstanten Abstand zur  *
// * rechten Wand einzuhalten                *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"

#define DISTANCE 400
#define GAIN1 1
#define GAIN2 4
#define MAXSPEED 255

/* Funktion zum Auslesen der Abstandssensoren (links, rechts) */
void DistanceData(unsigned int *data)
{
    // Prozessorinterne Referenz verwenden
    // Multiplexer auf linken Sensor schalten
    ADMUX = (1 << REFS0) | (1 << REFS1) | IR_LEFT;	
    // Wandlung starten
    ADCSRA |= (1 << ADSC);
    // Warten, bis Wandlung beendet			        
    while (!(ADCSRA & (1 << ADIF)));	                
    // ADCIF zurücksetzen
    ADCSRA |= (1 << ADIF);			        
    // ADC-Wert auslesen
    data[0] = ADCL + (ADCH << 8); // normale Sensoranordnung
    // data[1] = ADCL + (ADCH << 8); // gekreuzte Sensoranordnung

    // Prozessorinterne Referenz verwenden
    // Multiplexer auf linken Sensor schalten
    ADMUX = (1 << REFS0) | (1 << REFS1) | IR_RIGHT;
    // Wandlung starten
    ADCSRA |= (1 << ADSC);
    // Warten, bis Wandlung beendet		
    while (!(ADCSRA & (1 << ADIF)));	
    // ADCIF zurücksetzen
    ADCSRA |= (1 << ADIF);
    // ADC-Wert auslesen
    data[1] = ADCL + (ADCH << 8); // normale Sensoranordnung
    // data[0] = ADCL + (ADCH << 8); // gekreuzte Sensoranordnung

}


int main(void)
{
    unsigned int data[2];
    signed int diff;
    Init();
    MotorDir(FWD, FWD);
    MotorSpeed(MAXSPEED, MAXSPEED);
    while(1) {
    	// Entfernungen einlesen
    	DistanceData(data);
	// Differenz zum Sollwert ausrechnen und gewichten
	diff=(data[1]-DISTANCE)*GAIN1;
	// Drehen, falls der linke Sensor ein nahes Hindernis sieht
	if (data[0]>DISTANCE) diff+=(data[0]-DISTANCE)*GAIN2;
	// Wert nach oben und unten begrenzen
	if (diff>MAXSPEED) diff=MAXSPEED;
	if (diff<-MAXSPEED) diff=-MAXSPEED;
	// wenn zu nah, linken Motor langsamer, ...
	if (diff>0) MotorSpeed(MAXSPEED-diff,MAXSPEED);
	// ... sonst rechten Motor langsamer
	else MotorSpeed(MAXSPEED,MAXSPEED+diff);
    }
    return 0;
}
