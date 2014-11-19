// *******************************************
// * Odometrie für ASURO                     *
// * Fährt exakt geradeaus, die gemessene    *
// * Regelabweichung wird für den nächsten   *
// * Start im EEPROM gespeichert             *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"
#include <eeprom.h>

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
    signed int difference=0;
    unsigned int distance=0;
    unsigned int eeread,eewrite;
    char eewriteflag=0;
    Init();
    // lies ein Word aus EEPROM
    eeread=eeprom_read_word((uint16_t*)0);
    // Wert auf Plausibilit� prfen
    if (eeread<512) {difference=(signed)eeread-256;}
    MotorDir(FWD, FWD);
    while(1) {
        // Helligkeitswerte der Lichtschranken auslesen
        OdometrieData(data);
        // Wechsel linker Sensor von niedrig auf hoch?
        if ((status[0]==LOW) && (data[0]>TRIGGERLEVEL+HYSTERESIS)) {
            status[0]=HIGH;
            difference++;
	    distance++;
	    eewriteflag=1;
        }
        // Wechsel linker Sensor von hoch auf niedrig?
        if ((status[0]==HIGH) && (data[0]<TRIGGERLEVEL-HYSTERESIS)) {
            status[0]=LOW;
            difference++;
	    distance++;
	    eewriteflag=1;
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
        
	// Alle 128 Odo-Ticks den Wert ins EEPROM schreiben
	if ((distance%128==0)&&(eewriteflag==1)) {
	    eewrite=(unsigned) (difference+256);
	    eeprom_write_byte((uint8_t *)0,eewrite&0xFF);
	    eeprom_write_byte((uint8_t *)1,eewrite>>8);
	}
    }
    return 0;
}
