#include <asuro.h>
#include <avr/delay.h>

#define asuro_init() Init()

///////////////////////////////////////////////////////////////////////////
uint16_t getDistance() {
  uint16_t distance = 0;
  cli();    /// disable all interrupts

  /// send 20µs Trigger "ping" 
  DDRD |= (1 << PD2);    // set PD2 to OUTPUT   !!!
  PORTD |= (1 << PD2);   // set PD2 High
  _delay_us(20);         // wait 20 milliseconds
  PORTD &= ~(1 << PD2);  // Reset PD2 to Low

  /// wait to pulse
  DDRD &= ~(1 << PD2);             // set PD2 to INPUT
  while( !( PIND & (1<<PD2) )){;} // wait until pulse startet
  while( PIND & (1<<PD2) ){     // count pulse width
    _delay_us(58);   // wait time for 1cm delay
    distance++;      // increment distance
  }//end 

  sei();            // reenable all interrupts
  return distance; // return distance
}//end getDistance()

///////////////////////////////////////////////////////////////////////////
int main(){
  uint16_t ultrasonic_distance = 0;

  asuro_init();

  for(;;){ // main loop
    ultrasonic_distance = getDistance();    /// get ultrasonic distance
    SerPrint("\r\n ultrasonic distance: "); /// print output
    PrintInt(ultrasonic_distance);
  }//end mainloop
}//end main
