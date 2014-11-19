/**********************************************************************
 * Toolbox-Modul
 * Tested for ATMEGA 8
 * Version:   0.5 
 *********************************************************************/
#include "toolbox.h"

//*********************************************************************//
void reset(void){
   //Führt per Watchdoc nach 15ms einen Reset aus.
    uart_putString("\r\n\r\n");
   wdt_enable(WDTO_15MS);
   _delay_ms(500);
}//end reset()

//*********************************************************************//
void help(void){
    wdt_enable(WDTO_8S);
    uart_putString("\r\n  *** Tasten im Keypad-Modus: ***");
    uart_putString("\r\n  Q/W = Winkel(Konvergenz) erhoehen bzw. senken");
    uart_putString("\r\n  A/S = Kamera Abstand erhoehen bzw. senken");
    uart_putString("\r\n  1,2,3,4,5 = Winkel Beispiele");
    uart_putString("\r\n  6 = Kamera Abstand um 200 erhoehen");
    uart_putString("\r\n  7 = Kamera Abstand um 200 senken");
    uart_putString("\r\n  8 = Schritt-Motor abstellen");
    uart_putString("\r\n  0 = Abstand auf 0 kalibrieren");
    uart_putString("\r\n");
    uart_putString("\r\n  *** Befehle ***");
    uart_putString("\r\n  KOMANDO [[+|-]WERT]");
    uart_putString("\r\n  $help         - Diese hilfe.");
    uart_putString("\r\n  $reset        - System wird neu gestartet.");
    uart_putString("\r\n  $winkel       - Gibt aktuellen winkel aus.");
    uart_putString("\r\n  $winkel WERT  - Setzt winkel auf WERT.");
    uart_putString("\r\n  $abstand      - Gibt aktuellen abstand aus.");
    uart_putString("\r\n  $abstand WERT - Setzt abstand auf WERT.");
    uart_putString("\r\n  + | -         - rellative aenderungen.");
    snprintf(uart_buffer, UART_BUFFER_SIZE, "\r\n  Servo Position(%d)", servo_pos);
    uart_putString(uart_buffer);
    snprintf(uart_buffer, UART_BUFFER_SIZE, "\r\n  Stepper (%d)", stepper_current_pos);
    uart_putString(uart_buffer);
    uart_putString("\r\n");
}//end help()

//********************************************************************//
void notaus(void){
    char c;
    int stepper_save = stepper_direktion;
    stepper_direktion = 0;
    STEPPER_PORT = 0x00;
    while(1){
        wdt_enable(WDTO_8S);

        c = uart_checkByte();
        if(c == 0x1b){//"ESC"
            reset();        
        }// end "ESC" 

        if(c == 0x0D){ //"ENTER"
            stepper_direktion = stepper_save;
            return;
        }//end "ENTER"
    }
}
//********************************************************************//
char myStrComp(char* str1, char* str2){ //Stringvergleich mit a=A...
    char erg = 0;
    while(!erg && *str1 && *str2){
        erg = (*str1) - (*str2);
        str1++; str2++; 
    }// end while
    erg = erg | ((*str1) - (*str2));
    return erg;
}//end StrComp


