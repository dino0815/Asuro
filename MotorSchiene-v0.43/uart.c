/***********************************************************************
 * UART-Modul
 * default:   19200 Baud 8N1   
 * Processor: ATMEGA 8
 * CLOCK:     16 MHz
 * Version:   0.5 
 **********************************************************************/
#include "uart.h"

//********************************************************************//
void uart_init(void){
  UBRRL = bauddivider;			            // set baud rate
  UBRRH = bauddivider >> 8;               // set baud rate
  UCSRA = 0;				                  // no U2X, MPCM
  UCSRC = 1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0;	// No Parity | 1 Stop Bit | 8 Data Bit
  UCSRB = 1<<RXEN  | 1<<TXEN  | 1<<RXCIE; // | 1<<TXCIE;//enable RX, TX and interr.
}

//********************************************************************//
char uart_checkByte(void){
  //IF(Byte Empfangen)THEN RETURN Byte ELSE RETURN 0x00
  return (UCSRA & 0x80) ? UDR : 0x00;
}
//********************************************************************//
char uart_getByte(void){
   while(!(UCSRA & 0x80)); // wait for received byte
   return UDR;
}
//********************************************************************//
void uart_putByte(char c){
  while( (UCSRA & 1<<UDRE) == 0 ){;}
  UDR = c;
}
//********************************************************************//
void uart_putString(char *s){
  while(*s) uart_putByte(*s++);
}
//********************************************************************//
void uart_getString(char* erg, const unsigned char max){
    char count = 0;
    char c = uart_getByte();
    while(c != 0x0D){ //Bis c = "Enter"
        if(c == 0x08 || c == 0x7F){ // Backspace
            if(count > 0){ //wenn min. ein Zeichen im Buffer
                erg--;
                *erg = 0x00;
                count--;
                uart_putString("\b \b"); // Löschen im Terminal
            }else{
                uart_putByte(0x07);//beep, buffer schon leer
            }//end if
        }else if(c == 0x1B){ // ESC
            _delay_ms(3);
            c = uart_getByte();
            c = uart_getByte();
        }else if(count < max && (c > 0x20 /*|| c == '$'*/) && c < 0x7F){
            *erg = c;
            erg++;
            count++;
            uart_putByte(c);//echo
        }else{
            uart_putByte(0x07);//beep, buffer schon voll
        }//end if
        c = uart_getByte();
    }//end while
    *erg = 0x00; //String Terminator!
}//end uart_getString

