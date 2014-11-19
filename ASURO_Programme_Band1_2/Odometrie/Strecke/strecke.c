// *******************************************
// * Odometrie für ASURO                     *
// * Fährt exakt geradeaus, aber nur ein     *
// * Stück                                   *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"

// Schwellwert fr die Hell/Dunkel-Unterscheidung
// Eventuell muss damit etwas variiert werden
#define TRIGGERLEVEL 600
#define HYSTERESIS 10
#define LOW 0
#define HIGH 1

int main(void)
{
    unsigned int data[2];
    signed int status[2]={0,0};
    // Startwert fr halbe Motorgeschwindigkeit
    signed int difference=0;
    unsigned int distance=0; 
    signed int speed;
    Init();
    while(1) {
	switch (distance) {
		// Am Anfang der Strecke Vollgas
		case 0: MotorDir(FWD,FWD); speed=255; break;
		// Am Ende langsamer werden ...
		case 150: speed=200; break;
		case 160: speed=150; break;
		// ... und stehen bleiben
		case 172: MotorDir(BREAK,BREAK); break;
	}
        // Helligkeitswerte der Lichtschranken auslesen
        OdometrieData(data);
        // Wechsel linker Sensor von niedrig auf hoch?
        if ((status[0]==LOW) && (data[0]>TRIGGERLEVEL+HYSTERESIS)) {
            status[0]=HIGH;
            difference++;
	    distance++;
        }
        // Wechsel linker Sensor von hoch auf niedrig?
        if ((status[0]==HIGH) && (data[0]<TRIGGERLEVEL-HYSTERESIS)) {
            status[0]=LOW;
            difference++;
	    distance++;
        }
        // Wechsel rechter Sensor von niedrig auf hoch?
        if ((status[1]==LOW) && (data[1]>TRIGGERLEVEL+HYSTERESIS)) {
            status[1]=HIGH;
            difference--;
        }
        // Wechsel rechter Sensor von hoch auf niedrig?
        if ((status[1]==HIGH) && (data[1]<TRIGGERLEVEL-HYSTERESIS)) {
            status[1]=LOW;
            difference--;
        }
        // zur Sicherheit: verhindern, dass der Differenzz�ler
        // den erlaubten Wertebereich verl�st
        if (difference<-speed) difference=-speed;
        if (difference>speed) difference=speed;
        
        // Status-LED noch entsprechend der erkannten Segmente
        // aufleuchten lassen, grn fr links, rot fr rechts
        StatusLED(status[0]+status[1]*2);
        
        // Z�ldifferenz passend auf die Motoren verteilen
        if (difference>0) MotorSpeed(speed-difference,speed);
        else MotorSpeed(speed,speed+difference);
        
    }
    return 0;
}
