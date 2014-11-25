//#include <avr/io.h>
#include <inttypes.h>
#include <avr/io.h>
//#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "asuro.h"
#include "uart.h"

#define SET_BIT(PORT, PIN)		  (PORT |=  (1<<(PIN)))
#define RESET_BIT(PORT, PIN)		(PORT &= ~(1<<(PIN)))
#define TOGGLE_BIT(PORT, PIN) 	(PORT ^=  (1<<(PIN)))

char resetfirst = 0;

/////////////////////////////////////////////////////////////////////////
void reset(void){
   //Führt per Watchdoc nach 15ms einen Reset aus.
//   wdt_enable(WDTO_15MS);
//   _delay_ms(500);
}//end reset()

/////////////////////////////////////////////////////////////////////////
ISR(USART_RX_vect){
  char c = UDR;
  if( c == '9' ){ 
    uart_putByte( '*' );
//    reset();
  }else{
    uart_putByte( c );
  }
}
/*
ISR(USART_RX_vect){
  char c = UDR;
  if( resetfirst != 0 ){
    if( c == 0x20 ){  
      uart_putString("\r\n*** RESET ***\r\n"); 
      reset();
    }
    resetfirst = 0;
    uart_add_rxbuf( 0x30 );  
    uart_add_rxbuf( c );  
  }else{
    if( c == 0x30 ){  
      resetfirst = 1;  
    }else{
      uart_add_rxbuf( c );  
    }
  }//end if(resetfirst)
}
*/
/////////////////////////////////////////////////////////////////////////
int main(void){
//	Init();
  uart_init();
  uart_putString( "ASURO 08 ready!" );
  sei();
	while(1){
/*
    if( uart_rBuffer_wpos > 0 ){
      char c = uart_rBuffer[uart_rBuffer_rpos];
      uart_rBuffer_rpos++;
      if( uart_rBuffer_wpos <= uart_rBuffer_rpos ){
        uart_rBuffer_rpos=0;
        uart_rBuffer_wpos=0;
      }

      if( c == '1' ) BackLED( ON , OFF );
      else if( c == '2' ) BackLED( OFF , ON );
      else if( c == '3' ) BackLED( OFF , OFF );
      else if( c == '4' ) BackLED( ON , ON );
      else{
        uart_putByte( c );
      }//end else
    }
*/
//    BackLED( ON , OFF );
    uart_putString( "L " );
    _delay_ms(500);  
//    BackLED( OFF , ON );
    uart_putString( "R " );
    _delay_ms(500);  ////
//    SerWrite( "Piep .. ", 8 );
/**/
  };
	return 0;
}
