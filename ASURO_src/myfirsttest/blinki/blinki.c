//#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
 
// LEDs sind active-high geschaltet
#define SET_BIT(PORT, PIN)		  (PORT |=  (1<<(PIN)))
#define RESET_BIT(PORT, PIN)		(PORT &= ~(1<<(PIN)))
#define TOGGLE_BIT(PORT, PIN) 	(PORT ^=  (1<<(PIN)))

//#define LED1			PD4
//#define LED2			PD5
//#define TASTER	  PB1
long int i=0; 

int main(void){
  DDRC |= (1<<PC0);// | (1<<LED2); // Port D: Ausgang für LED1 und LED2
  SET_BIT(PORTC, PC0);
 
  while(1)  {
//    if(i>10000){
      _delay_ms(1000);  ////
      TOGGLE_BIT(PORTC, PC0);
//      i=0;
//    }else{
//      i++;
//    }//end if
  }
}

