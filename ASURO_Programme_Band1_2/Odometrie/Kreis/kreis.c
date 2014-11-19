// *******************************************
// * Odometrie für ASURO                     *
// * Fährt einen Kreis mit 31cm Durchmesser  *
// *                                         *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"

// Schwellwert für die Hell/Dunkel-Unterscheidung
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
    signed int difference=87; 
    Init();
    MotorDir(FWD, FWD);
    while(1) {
        // Helligkeitswerte der Lichtschranken auslesen
        OdometrieData(data);
        // Wechsel linker Sensor von niedrig auf hoch?
        if ((status[0]==LOW) && (data[0]>TRIGGERLEVEL+HYSTERESIS)) {
            status[0]=HIGH;
            difference+=2;
        }
        // Wechsel linker Sensor von hoch auf niedrig?
        if ((status[0]==HIGH) && (data[0]<TRIGGERLEVEL-HYSTERESIS)) {
            status[0]=LOW;
            difference+=2;
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
        if (difference<-255) difference=-255;
        if (difference>255) difference=255;
        
        // Status-LED noch entsprechend der erkannten Segmente
        // aufleuchten lassen, grn fr links, rot fr rechts
        StatusLED(status[0]+status[1]*2);
        
        // Z�ldifferenz passend auf die Motoren verteilen
        if (difference>0) MotorSpeed(255-difference,255);
        else MotorSpeed(255,255+difference);
        
    }
    return 0;
}
