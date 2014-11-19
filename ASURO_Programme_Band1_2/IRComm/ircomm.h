// ircomm.h
// Routines for easier communication
// via the IR-Transceiver
// (c) 2006 Robin Gruber


#ifndef IRCOMM_H
#define IRCOMM_H

#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/io.h>

#define TXBUFFERSIZE 256
#define RXBUFFERSIZE 32

#define F_CPU 8000000l

#define BLOCKING 1
#define NONBLOCKING 0

void IRCommInit(long unsigned int baudrate);
int IRCommTransmit(char * c, int length, int blocking);
int IRCommReceive(char *c, int length, int blocking);
int IRCommTransmissionComplete(void);
int IRCommGetBytesInRXBuffer(void);
int IRCommGetBytesInTXBuffer(void);


#endif
