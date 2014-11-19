//#include <avr/io.h>
#include "asuro.h"
#include <util/delay.h>

#define SET_BIT(PORT, PIN)		  (PORT |=  (1<<(PIN)))
#define RESET_BIT(PORT, PIN)		(PORT &= ~(1<<(PIN)))
#define TOGGLE_BIT(PORT, PIN) 	(PORT ^=  (1<<(PIN)))

int main(void){
	Init();
//  DDRC |= (1<<PC0);// | (1<<LED2); // Port D: Ausgang für LED1 und LED2
//  SET_BIT(PORTC, PC0);

	while(1){
    BackLED( ON , OFF );
      _delay_ms(200);  ////
    BackLED( OFF , ON );
      _delay_ms(200);  ////
    SerWrite( "Piep .. ", 8 );

  };
	return 0;
}
