// *******************************************
// * Small test for the IR-Communication-    *
// * Routines                                *
// *                                         *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * More details can be found in            *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "asuro.h"
#include "ircomm.h"


#define BUFFERSIZE 32

int main(void)
{
	int i;
	char c[BUFFERSIZE];
	int bytesRead;
	Init();
	IRCommInit(2400);
        do {
	  // Call "Hello" till we get an answer!
	        IRCommTransmit("Hello?\n\r",8,BLOCKING);
                for (i=0; i<2000; i++) Sleep(72);
	}
	while (IRCommReceive(c,BUFFERSIZE,NONBLOCKING)==0);
	while (1) {
		// Echo the data but with blocking, we don't want to lose data.
		IRCommTransmit("You typed:", 10, BLOCKING);
		IRCommTransmit(c, bytesRead, BLOCKING);
		IRCommTransmit("\n\r", 2, BLOCKING);
	}
}
