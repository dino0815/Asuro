// *******************************************
// * Routines for easier communication via   *
// * the IR-TRansceiver                      *
// *                                         *
// *                                         *
// * (c) 2007 Robin Gruber                   *
// * More details can be found in            *
// * "Mehr Spaß mit ASURO, Band II"          *
// *******************************************

#include "ircomm.h"
#include "asuro.h"

volatile char rxbuffer[TXBUFFERSIZE];
volatile char txbuffer[RXBUFFERSIZE];
volatile int rxpointer0=0, rxpointer1=0;
volatile int txpointer0=0, txpointer1=0;

volatile int txcomplete;

/* Initiate communication, special baudrates can be set */
void IRCommInit(long unsigned int baudrate)
{
	// Init USART
	UCSRA=0x00;
	// receiver and RX interrupt on
	UCSRB=(1<<RXCIE) | (1<<RXEN);
	UCSRC=(1<<UCSZ1) | (1<<UCSZ0) | (1<<URSEL);
	UBRRH=(((F_CPU/baudrate)>>4) - 1) >> 8;
	UBRRL=(((F_CPU/baudrate)>>4) - 1) & 0xFF;
	txcomplete=TRUE;
}

int IRCommTransmit(char * c, int length, int blocking)
{
	int i;
	for (i=0; i<length; i++) {
		// Buffer full?
		if ((TXBUFFERSIZE+txpointer1-txpointer0)%TXBUFFERSIZE > (TXBUFFERSIZE-2)) {
			// if blocking is enabled wait till the buffer has space
			if (blocking) while ((TXBUFFERSIZE+txpointer1-txpointer0)%TXBUFFERSIZE > (TXBUFFERSIZE-2));
			// else return immediately, the remaining data is lost then!
			else return i;
		}
		txcomplete=FALSE;
		// fill the bytes to transmit into the transmit buffer
		// and increase the pointer where to write
		asm("cli");
		txbuffer[txpointer1++]=c[i];
		// wrapping of pointer
		if (txpointer1 > TXBUFFERSIZE-1) txpointer1=0;
		asm("sei");
		// At least one byte has been filled in, so...
		// ... enable the transmitter and the RX interrupt and ...
		// ... disable the receiver and the RX interrupt
		UCSRB=(1<<TXEN)|(1<<UDRIE);
	}
	return length;
}

int IRCommReceive(char *c, int length, int blocking)
{
	int i;
	for (i=0; i<length; i++) {
		if (rxpointer1==rxpointer0) {
			if (blocking) while (rxpointer1==rxpointer0); else return i;
		} 
		c[i]=rxbuffer[rxpointer1++];
		if (rxpointer1 > RXBUFFERSIZE - 1) rxpointer1=0;
	}
	return length;
}

int IRCommTransmissionComplete(void)
{
	return txcomplete;
}

int IRCommGetBytesInRXBuffer(void)
{
	return (RXBUFFERSIZE+rxpointer1-rxpointer0)%RXBUFFERSIZE;
}

int IRCommGetBytesInTXBuffer(void)
{
	return (TXBUFFERSIZE+txpointer1-txpointer0)%TXBUFFERSIZE;
}

SIGNAL (SIG_UART_RECV)
{
	rxbuffer[rxpointer0++]=UDR;
	if (rxpointer0 > RXBUFFERSIZE-1) rxpointer0 = 0;
}

SIGNAL (SIG_UART_DATA)
{
	// fill UDR with new byte to transmit
	UDR=txbuffer[txpointer0++];
	if (txpointer0 > TXBUFFERSIZE-1) txpointer0 = 0;
	// If transmit pointers are equal disable data register empty interrupt
	// and enable TX complete interrupt to notice when all data is out
	if (txpointer0==txpointer1) UCSRB = (1<<TXEN) | (1<<TXCIE); 
}

SIGNAL (SIG_UART_TRANS)
{
	// The last byte has been shifted out
	// disable transmitter and transmit interrupts enable the receiver again
	UCSRB = (1<<RXCIE) | (1<<RXEN);
	txcomplete=TRUE;
}


