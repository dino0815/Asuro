#ifndef _UART_H_
#define _UART_H_

//#include "toolbox.h"
#include <avr/io.h>
#include <util/delay.h>


#define bauddivider (unsigned int)(F_CPU / BAUD / 16 - 0.5)

#define UART_BUFFER_SIZE 80
extern char uart_buffer[UART_BUFFER_SIZE];

#define UART_RBUFFER_SIZE 8
extern volatile char uart_rBuffer[UART_RBUFFER_SIZE];
extern volatile unsigned char uart_rBuffer_rpos; //lese Position
extern volatile unsigned char uart_rBuffer_wpos; //schreibe Position

void uart_init(void);
char uart_checkByte(void); // Prüft und gibt sonst Null zurück
char uart_getByte(void);   // Wartet bis Byte empfangen wurde
void uart_putByte(char c); // Sendet ein Byte
void uart_getString(char* erg, unsigned char max); //Blockiert bis String mit "Enter" abgeschlossen wird
void uart_putString(char *s); // Sendet Null-Terminierten String

void uart_add_rxbuf( char c );

/*
// Umbenenungen damit der Quellcode statt mit einem ATmega32 auch mit ATmega644 funktioniert.
//#if defined(__AVR_ATMEGA644__) 
    #define UDR            UDR0
    #define UDRE           UDRE0 
    #define UBRRL          UBRR0L 
    #define UBRRH          UBRR0H 
    #define UCSRA          UCSR0A 
    #define UCSRB          UCSR0B 
    #define UCSRC          UCSR0C 
    #define URSEL          0x00 
    #define UCSZ1          UCSZ01 
    #define UCSZ0          UCSZ00 
    #define RXEN           RXEN0 
    #define TXEN           TXEN0 
    #define RXCIE          RXCIE0 
    #define USART_RX_vect  USART0_RX_vect
//    #define RXC RXC0 
//#endif
*/
#endif
