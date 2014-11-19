#include <avr/io.h>
//#include <util/delay.h>
 
// LEDs sind active-high geschaltet
#define SET_HIGH(PIN, PORT)	 (PORT |=  (1<<(PIN)))
#define SET_LOW(PIN, PORT)	 (PORT &= ~(1<<(PIN)))
#define TOGGLE(PIN, PORT)	(PORT ^=  (1<<(PIN)))

#define SET_OUTPUT(PIN, PORT)	 (PORT |=  (1<<(PIN)))
#define SET_INPUT(PIN, PORT)	 (PORT &= ~(1<<(PIN)))

#define LED1			PC0
#define LED_PORT	PORTC

int main(void){
//  SET_OUTPUT(LED1, LED_PORT);
  DDRC = 0xFF;   
//  while(1)  {
loop:
      //_delay_ms(10000);  ////
//      TOGGLE(LED1, LED_PORT);
      PORTC =  0xFF;
      PORTC =  0x00;
      PORTC =  0xFF;
      PORTC =  0x00;
      PORTC =  0xFF;
      PORTC =  0x00;
      PORTC =  0xFF;
      PORTC =  0x00;
      PORTC =  0xFF;
      PORTC =  0x00;
      PORTC =  0xFF;
      PORTC =  0x00;
goto loop;
//  }
}

