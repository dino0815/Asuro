#include <asuro.h>
#include <avr/wdt.h>
#include <avr/delay.h>

#define asuro_init() Init()
#define SPEED 150

////////////////////////////////////////////////////////////////////////////
void reset(void){
  //Führt per Watchdoc nach 15ms einen Reset aus.
  wdt_enable(WDTO_15MS);
  _delay_ms(500);
}//end reset()

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
void PrintSensorData( void ){
  uint16_t ultrasonic_distance = 0;
  uint16_t line_lumina[2];

  FrontLED(ON);
  LineData( line_lumina );
  ultrasonic_distance = getDistance();    /// get ultrasonic distance

  SerPrint( " ultrasonic distance: " ); /// print output
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
}

/* //////////////////////////////////////////////////////////////////////////
void help( void ){
  SerPrint( "\n\r Zu Pfeiltasten für:" ); // Next line
  SerPrint( "forward" ); // Next line
  SerPrint( "backward" ); // Next line
  SerPrint( "counter-clockwise" ); // Next line
  SerPrint( "clockwise" ); // Next line
  SerPrint( "break" ); // Next line
  SerWrite( , 1 );
  SerPrint( "\n\r" ); // Next line
  _delay_ms( 2000 );
}
*/
///////////////////////////////////////////////////////////////////////////
void RemoteControll( void ){
  unsigned char buffer = 0x00;     /// init variables

  /// read single byte from serial port (non-blocking)
  SerRead(&buffer, 1, 500);

  /// continue main loop on received timeout
  if( buffer == 'T' ){
    return;
  }else{
		SerPrint( "\n\r Command: " ); // Next line
		SerWrite( &buffer, 1 );
	//  SerPrint( "\n\r" ); // Next line
  }

  /// switch byte
  switch( buffer ){

  /// drive forward on 0xDE
  case 'w':  //0xDE:
      MotorDir(FWD, FWD);
      MotorSpeed(SPEED, SPEED);
      break;

  /// drive backward on 0xAD
  case 's':  //0xAD:
      MotorDir(RWD, RWD);
      MotorSpeed(SPEED, SPEED);
      break;

  /// turn counter-clockwise  0xBE
  case 'a':  //0xBE:
      MotorDir(RWD, FWD);
      MotorSpeed(SPEED, SPEED);
      break;

  /// turn clockwise 0xEF
  case 'd':  //0xEF:
      MotorDir(FWD, RWD);
      MotorSpeed(SPEED, SPEED);
      break;

  /// break 0xFF
  case ' ':  //0xFF:
      MotorDir(BREAK, BREAK);
      MotorSpeed(BREAK, BREAK);
      break;

  /// break on default
  default:
      MotorDir(BREAK, BREAK);
      MotorSpeed(BREAK, BREAK);
      break;
  }
}

///////////////////////////////////////////////////////////////////////////
int main(){
  asuro_init();

  for(;;){ // main loop
//    SerPrint( "\n\r"); // Next line
    RemoteControll();  // Get commands from serial  
//    PrintSensorData(); // Get sensordata and send it to serial
  }//end mainloop
}//end main

