#include <asuro.h>
#include <avr/delay.h>

#define asuro_init() Init()

///////////////////////////////////////////////////////////////////////////
uint16_t getDistance( void ) {
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
  uint16_t line_lumina[2];

  asuro_init();
  FrontLED(ON);

  for(;;){ // main loop
    LineData( line_lumina );
    ultrasonic_distance = getDistance();    /// get ultrasonic distance

    SerPrint( "\n\r ultrasonic distance: " ); /// print output
    if( ultrasonic_distance < 1000 ) SerPrint( " " );
    if( ultrasonic_distance < 100 ) SerPrint( " " );
    if( ultrasonic_distance < 10 ) SerPrint( " " );
    PrintInt( ultrasonic_distance );

    SerPrint( " ;   LineData( left | right ): " );
    if( line_lumina[0] < 100 ) SerPrint( " " );
    if( line_lumina[0] < 10 ) SerPrint( " " );
    PrintInt( line_lumina[0] );
    SerPrint( " ,  " );
    if( line_lumina[1] < 100 ) SerPrint( " " );
    if( line_lumina[1] < 10 ) SerPrint( " " );
    PrintInt( line_lumina[1] );

  }//end mainloop
}//end main
