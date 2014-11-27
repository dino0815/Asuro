#include <asuro.h>
#include <avr/delay.h>

#define asuro_init() Init()

///////////////////////////////////////////////////////////////////////////
int main(){
  uint16_t line_lumina[2];

  asuro_init();
  FrontLED(ON);

  for(;;){ // main loop
    LineData( line_lumina );

    SerPrint( "\n\r LineData( left | right ): " );
    if( line_lumina[0] < 100 ) SerPrint( " " );
    if( line_lumina[0] < 10 ) SerPrint( " " );
    PrintInt( line_lumina[0] );
    SerPrint( " ,  " );
    if( line_lumina[1] < 100 ) SerPrint( " " );
    if( line_lumina[1] < 10 ) SerPrint( " " );
    PrintInt( line_lumina[1] );

  }//end mainloop
}//end main
