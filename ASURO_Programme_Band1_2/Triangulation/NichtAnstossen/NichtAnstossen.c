// *******************************************
// * Triangulation für ASURO                 *
// *                                         *
// * Versucht, nicht an der Wand anzustossen *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"

#define LEDLEVEL 200
#define WARNINGLEVEL 300

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
    data[0] = ADCL + (ADCH << 8);// normale Sensoranordnung
    // data[1] = ADCL + (ADCH << 8);// gekreuzte Sensoranordnung

    // Prozessorinterne Referenz verwenden
    // Multiplexer auf rechten Sensor schalten
    ADMUX = (1 << REFS0) | (1 << REFS1) | IR_RIGHT;
    // Wandlung starten
    ADCSRA |= (1 << ADSC);
    // Warten, bis Wandlung beendet		
    while (!(ADCSRA & (1 << ADIF)));	
    // ADCIF zurücksetzen
    ADCSRA |= (1 << ADIF);
    // ADC-Wert auslesen
    data[1] = ADCL + (ADCH << 8);// normale Sensoranordnung
    // data[0] = ADCL + (ADCH << 8);// gekreuzte Sensoranordnung
}


int main(void)
{
    unsigned int data[2];
    int leftled, rightled;
    signed int diff;
    int flag=0;
    Init();
    MotorDir(FWD, FWD);
    MotorSpeed(255, 255);
    while(1) {
    	// Entfernungen einlesen
    	DistanceData(data);
        // LEDs einschalten wenn ein bestimmter Abstand 
	// unterschritten wird
	if (data[1]>LEDLEVEL) leftled=ON; else leftled=OFF;
	if (data[0]>LEDLEVEL) rightled=ON; else rightled=OFF;
	BackLED(leftled,rightled);

	// sind wir an beiden Seiten schon zu nah?
        if (data[0]>WARNINGLEVEL && data[1]>WARNINGLEVEL) {
	    StatusLED(RED);
            MotorSpeed(255,255);
	    // auf der Stelle drehen, um nicht anzustoßen...
            if (flag==0){
	    	// ... und zwar in die Richtung, in die mehr Platz ist
		// außerdem die Drehrichtung merken
	    	if (data[0]>data[1]) {MotorDir(FWD,RWD); flag=1;}
                else { MotorDir(RWD,FWD); flag=2;}
            }
            else {
	    	// Wenn wir bereits umdrehen, dann die Richtung
		// beibehalten, auch wenn sich die Abstände
		// inzwischen geändert haben
                if (flag==1) MotorDir(FWD,RWD);
                else MotorDir(RWD,FWD);
                }
        } else {
	    // Abstand reicht noch?
	    StatusLED(GREEN);
            MotorDir(FWD,FWD);
            flag=0;
	    // Dann möglichst die Mitte halten
            diff=((signed int)data[0]-(signed int)data[1])/2;
            if (diff>255) diff=255;
            if (diff<-255) diff=-255;
            if (diff>0) MotorSpeed(255,255-diff);
            else MotorSpeed(255+diff, 255);
	}
  }
  return 0;
}
