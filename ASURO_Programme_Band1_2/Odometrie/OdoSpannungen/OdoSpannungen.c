// *******************************************
// * Odometrie für ASURO                     *
// * Misst die Odometerspannungen und gibt   *
// * sie über die Infrarotschnittstelle aus  *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * Details zum Code in:                    *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"
#include <string.h>
#include <stdio.h>

int main(void)
{
	unsigned int data[200][2];
	char c[12];
	int i;
	Init();

	// Motoren auf Vorw�ts 
	MotorDir(FWD, FWD);
	MotorSpeed(160,160);
	// Warten, bis Motoren Drehzahl erreicht haben
	for (i=0; i<1000; i++) Sleep(72);
	
	// 200 Odometrie-Werte einlesen
    	for (i=0; i<200; i++) {
		OdometrieData(data[i]);
	}

	MotorSpeed(0,0);

    	for (i=0; i<200; i++) {
		sprintf(c,"%4d\t%4d\n\r",data[i][0],data[i][1]);
		SerWrite(c,12);
	}


	while(1) {
	}
	return 0;
}
